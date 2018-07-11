// This file is part of LatNet Builder.
//
// Copyright (C) 2012-2018  Pierre L'Ecuyer and Universite de Montreal
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef LATBUILDER__MERIT_SEQ__CONCRETE_COORD_SYM_STATE_IPOD_H
#define LATBUILDER__MERIT_SEQ__CONCRETE_COORD_SYM_STATE_IPOD_H

#include "latbuilder/MeritSeq/CoordUniformState.h"
#include "latbuilder/Storage.h"
#include "latbuilder/Kernel/AIDNAlpha.h"
#include "latbuilder/Kernel/BIDN.h"

#include "netbuilder/Interlaced/IPODWeights.h"

#include <vector>

namespace LatBuilder { namespace MeritSeq {

// forward declaration
template <LatticeType LR, EmbeddingType ET, Compress COMPRESS , PerLevelOrder PLO, class WEIGHTS> class ConcreteCoordUniformState;

/**
 * Implementation of CoordUniformState for POD weights.
 */
template <LatticeType LR, EmbeddingType ET, Compress COMPRESS, PerLevelOrder PLO, class KERNEL>
class ConcreteCoordUniformState<LR, ET, COMPRESS, PLO, NetBuilder::Interlaced::IPODWeights<KERNEL>> :
   public CoordUniformState<LR, ET, COMPRESS, PLO> {
public:
   /**
    * Constructor.
    * 
    * Sets \f$\mathcal D_0^2 = 0\f$ and \f$\boldsymbol p_{0,\ell} =
    * \boldsymbol 1\f$.
    *
    * \param storage       Storage configuration.
    * \param weights       POD weights
    */
   ConcreteCoordUniformState(
         const Storage<LR, ET, COMPRESS, PLO>& storage,
         const NetBuilder::Interlaced::IPODWeights<KERNEL>& weights
         ):
      CoordUniformState<LR, ET, COMPRESS, PLO>(storage),
      m_weights(weights),
      m_interlacingFactor(m_weights.getInterlacingFactor())
   { reset();}

   void reset();

   /**
    * Updates the current state using the specified row of the permuted matrix      
    * of kernel values.           
    * This corresponds to appending a component \f$a_j\f$ to the generating      
    * vector \f$\boldsymbol a = (a_1, \dots, a_{j-1})\f$.      
    * To each possible value of \f$a_j\f$ corresponds a distinct row of the      
    * matrix \f$\boldsymbol\Omega\f$ of kernel values.           
    * This increases the internal dimension counter.
    *
    * Computes
    * \f[
    *    \boldsymbol p_{s,\ell} =
    *       \boldsymbol p_{s-1,\ell} + \boldsymbol\omega_s \odot \boldsymbol p_{s-1,\ell-1}.
    * \f]
    */
   void update(const RealVector& kernelValues, typename LatticeTraits<LR>::GenValue gen);

   /**
    * Computes and returns the weighted state vector \f$\boldsymbol q_s\f$.
    *
    * Computes
    * \f[
    *    \boldsymbol q_s = \sum_{\ell=0}^s \Gamma_{\ell+1} \boldsymbol p_{s,\ell}.
    * \f]
    */
   RealVector weightedState() const;

      /**
    * Returns a copy of this instance.
    */
   std::unique_ptr<CoordUniformState<LR, ET, COMPRESS, PLO>> clone() const
   { return std::unique_ptr<CoordUniformState<LR, ET, COMPRESS, PLO>>(new ConcreteCoordUniformState(*this)); }

private:
   const NetBuilder::Interlaced::IPODWeights<KERNEL>& m_weights;
   unsigned int m_interlacingFactor;

   // m_state[level](i)
   RealVector m_ElemPolySum;
   RealVector m_ODSum;
   std::vector<RealVector> m_state;
};

extern template class ConcreteCoordUniformState<LatticeType::POLYNOMIAL, EmbeddingType::UNILEVEL, Compress::NONE, PerLevelOrder::BASIC,       NetBuilder::Interlaced::IPODWeights<Kernel::AIDNAlpha>>;
extern template class ConcreteCoordUniformState<LatticeType::POLYNOMIAL, EmbeddingType::MULTILEVEL, Compress::NONE, PerLevelOrder::CYCLIC,       NetBuilder::Interlaced::IPODWeights<Kernel::AIDNAlpha>>;

extern template class ConcreteCoordUniformState<LatticeType::POLYNOMIAL, EmbeddingType::MULTILEVEL, Compress::NONE, PerLevelOrder::BASIC,       NetBuilder::Interlaced::IPODWeights<Kernel::AIDNAlpha>>;

extern template class ConcreteCoordUniformState<LatticeType::DIGITAL, EmbeddingType::UNILEVEL, Compress::NONE, PerLevelOrder::BASIC,       NetBuilder::Interlaced::IPODWeights<Kernel::AIDNAlpha>>;
extern template class ConcreteCoordUniformState<LatticeType::DIGITAL, EmbeddingType::MULTILEVEL, Compress::NONE, PerLevelOrder::BASIC,  NetBuilder::Interlaced::IPODWeights<Kernel::AIDNAlpha>>;


extern template class ConcreteCoordUniformState<LatticeType::POLYNOMIAL, EmbeddingType::UNILEVEL, Compress::NONE, PerLevelOrder::BASIC,       NetBuilder::Interlaced::IPODWeights<Kernel::BIDN>>;
extern template class ConcreteCoordUniformState<LatticeType::POLYNOMIAL, EmbeddingType::MULTILEVEL, Compress::NONE, PerLevelOrder::CYCLIC,       NetBuilder::Interlaced::IPODWeights<Kernel::BIDN>>;

extern template class ConcreteCoordUniformState<LatticeType::POLYNOMIAL, EmbeddingType::MULTILEVEL, Compress::NONE, PerLevelOrder::BASIC,       NetBuilder::Interlaced::IPODWeights<Kernel::BIDN>>;

extern template class ConcreteCoordUniformState<LatticeType::DIGITAL, EmbeddingType::UNILEVEL, Compress::NONE, PerLevelOrder::BASIC,       NetBuilder::Interlaced::IPODWeights<Kernel::BIDN>>;
extern template class ConcreteCoordUniformState<LatticeType::DIGITAL, EmbeddingType::MULTILEVEL, Compress::NONE, PerLevelOrder::BASIC,  NetBuilder::Interlaced::IPODWeights<Kernel::BIDN>>;

}}

#endif
