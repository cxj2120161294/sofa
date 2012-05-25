/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*                (c) 2006-2011 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Plugins                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_VolumePreservationFORCEFIELD_H
#define SOFA_VolumePreservationFORCEFIELD_H

#include "../initFlexible.h"
#include "../material/BaseMaterialForceField.h"
#include "../material/VolumePreservationMaterialBlock.inl"

#include <sofa/helper/OptionsGroup.h>

namespace sofa
{
namespace component
{
namespace forcefield
{

using helper::vector;
using helper::OptionsGroup;

/** Apply VolumePreservation's Law for isotropic homogeneous incompressible materials.
  * The energy is : k/2 ln( I3^1/2 )^2
*/

template <class _DataTypes>
class SOFA_Flexible_API VolumePreservationForceField : public BaseMaterialForceField<defaulttype::VolumePreservationMaterialBlock<_DataTypes> >
{
public:
    typedef defaulttype::VolumePreservationMaterialBlock<_DataTypes> BlockType;
    typedef BaseMaterialForceField<BlockType> Inherit;

    SOFA_CLASS(SOFA_TEMPLATE(VolumePreservationForceField,_DataTypes),SOFA_TEMPLATE(BaseMaterialForceField, BlockType));

    typedef typename Inherit::Real Real;

    /** @name  Material parameters */
    //@{
    Data<OptionsGroup> f_method;
    Data<Real> f_k;
    //@}

    virtual void reinit()
    {
        for(unsigned int i=0; i<this->material.size(); i++) this->material[i].init(this->f_k.getValue());
        Inherit::reinit();
    }

protected:
    VolumePreservationForceField(core::behavior::MechanicalState<_DataTypes> *mm = NULL)
        : Inherit(mm)
        , f_method ( initData ( &f_method,"method","energy form" ) )
        , f_k(initData(&f_k,(Real)0,"k","bulk modulus: weight ln(J)^2/2 term in energy "))
    {
        helper::OptionsGroup Options(2	,"0 - k.ln(J)^2/2"
                ,"1 - k.(J-1)^2/2" );
        Options.setSelectedItem(1);
        f_method.setValue(Options);
    }

    virtual ~VolumePreservationForceField() {}



    virtual void addForce(const core::MechanicalParams* /*mparams*/ /* PARAMS FIRST */, typename Inherit::DataVecDeriv& _f , const typename Inherit::DataVecCoord& _x , const typename Inherit::DataVecDeriv& _v)
    {
        typename Inherit::VecDeriv&  f = *_f.beginEdit();
        const typename Inherit::VecCoord&  x = _x.getValue();
        const typename Inherit::VecDeriv&  v = _v.getValue();

        switch( f_method.getValue().getSelectedId() )
        {
        case 0:
        {
            for( unsigned int i=0 ; i<this->material.size() ; i++ )
            {
                this->material[i].addForce_method0(f[i],x[i],v[i]);
            }
            break;
        }
        case 1:
        {
            for( unsigned int i=0 ; i<this->material.size() ; i++ )
            {
                this->material[i].addForce_method1(f[i],x[i],v[i]);
            }
            break;
        }
        }

        _f.endEdit();

        if(!BlockType::constantK)
        {
            if(this->assembleC.getValue()) this->updateC();
            if(this->assembleK.getValue()) this->updateK();
            if(this->assembleB.getValue()) this->updateB();
        }
    }

    virtual void addDForce(const core::MechanicalParams* mparams /* PARAMS FIRST */, typename Inherit::DataVecDeriv&   _df , const typename Inherit::DataVecDeriv&   _dx )
    {
        if(this->assembleK.getValue())
        {
            this->K.addMult(_df,_dx,mparams->kFactor());
            if(this->assembleB.getValue())   this->B.addMult(_df,_dx,mparams->bFactor());
        }
        else
        {
            typename Inherit::VecDeriv&  df = *_df.beginEdit();
            const typename Inherit::VecDeriv&  dx = _dx.getValue();

            switch( f_method.getValue().getSelectedId() )
            {
            case 0:
            {
                for( unsigned int i=0 ; i<this->material.size() ; i++ )
                {
                    this->material[i].addDForce_method0(df[i],dx[i],mparams->kFactor(),mparams->bFactor());
                }
                break;
            }
            case 1:
            {
                for( unsigned int i=0 ; i<this->material.size() ; i++ )
                {
                    this->material[i].addDForce_method1(df[i],dx[i],mparams->kFactor(),mparams->bFactor());
                }
                break;
            }
            }

            _df.endEdit();
        }
    }





};


}
}
}

#endif
