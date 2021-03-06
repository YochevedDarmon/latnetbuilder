// This file is part of LatNet Builder.
//
// Copyright (C) 2012-2021  The LatNet Builder author's, supervised by Pierre L'Ecuyer, Universite de Montreal.
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

#ifndef LATBUILDER__GENSEQ__CYCLIC_GROUP_PLR_H
#define LATBUILDER__GENSEQ__CYCLIC_GROUP_PLR_H

#include "latbuilder/GenSeq/CyclicGroup.h"



namespace LatBuilder { namespace GenSeq {

  template<>
  struct CyclicGroupTraversalTraits<CyclicGroupTraversal<LatticeType::POLYNOMIAL>>{
    typedef size_t size_type;
    typedef LatticeTraits<LatticeType::POLYNOMIAL>::GenValue value_type;
    typedef LatticeTraits<LatticeType::POLYNOMIAL>::Modulus Modulus;
      static void increment(value_type& currentValue, size_type& index, const value_type& generator, const Modulus& modulus, const size_type& size){
      index ++;
      currentValue = (currentValue * generator) % modulus ;
      
      }
  };

  template<Compress COMPRESS,class TRAV, GroupOrder ORDER>
  struct CyclicGroupTraits<CyclicGroup<LatticeType::POLYNOMIAL,COMPRESS,TRAV,ORDER>>{
    typedef size_t size_type;
    typedef LatticeTraits<LatticeType::POLYNOMIAL>::GenValue value_type;
    typedef LatticeTraits<LatticeType::POLYNOMIAL>::Modulus Modulus;
  };

template <Compress COMPRESS,
         class TRAV ,
         GroupOrder ORDER >
class CyclicGroup< LatticeType::POLYNOMIAL, COMPRESS, TRAV, ORDER> :
   public Traversal::Policy<CyclicGroup<LatticeType::POLYNOMIAL, COMPRESS, TRAV, ORDER>, TRAV> {

   typedef CyclicGroup<LatticeType::POLYNOMIAL, COMPRESS, TRAV, ORDER> self_type;
   typedef Traversal::Policy<self_type, TRAV> TraversalPolicy;
   typedef CompressTraits<COMPRESS> Compress;

public:
   typedef typename CyclicGroupTraits<self_type>::size_type size_type;
   typedef typename CyclicGroupTraits<self_type>::value_type value_type;
   typedef typename CyclicGroupTraits<self_type>::Modulus Modulus;

   static constexpr LatBuilder::Compress compress() { return COMPRESS; }

   /**
    * Traversal type.
    */
   typedef TRAV Traversal;

   static std::string name()
   { return std::string("cyclic group / ") + Compress::name() + " / " + Traversal::name(); }

   /**
    * Constructor for an empty group.
    */
   CyclicGroup(Traversal trav = Traversal()):
      TraversalPolicy(std::move(trav)),
      m_base(0), m_power(0), m_modulus(0), m_gen(0) {}

   /**
    * Constructor for the cyclic group of integers modulo \f$b^m\f$, where
    * \f$b\f$ is a prime base.
    * \param base       irreductible base \f$b\f$.
    * \param power      Power \f$m\f$ of the base. (if power \f$>1\f$, an exeption is raised)
    * \param trav       Traversal instance.
    */
   CyclicGroup(Modulus base, Level power, Traversal trav = Traversal());

   /**
    * Cross-traversal copy-constructor.
    */
   template <class TRAV2>
   CyclicGroup(
         const CyclicGroup<LatticeType::POLYNOMIAL, COMPRESS, TRAV2, ORDER>& other,
         Traversal trav = Traversal()):
      TraversalPolicy(std::move(trav)),
      m_base(other.m_base),
      m_power(other.m_power),
      m_modulus(other.m_modulus),
      m_gen(other.m_gen)
   {}

   /**
    * Rebinds the traversal type.
    */
   template <class TRAV2>
   struct RebindTraversal {
      typedef CyclicGroup<LatticeType::POLYNOMIAL, COMPRESS, TRAV2, ORDER> Type;
   };

   /**
    * Returns a copy of this object, but using a different traversal policy.
    */
   template <class TRAV2>
   typename RebindTraversal<TRAV2>::Type rebind(TRAV2 trav) const
   { return typename RebindTraversal<TRAV2>::Type{*this, std::move(trav)}; }

   /**
    * Returns the base of the group modulus.
    */
   Modulus base() const
   { return m_base; }

   /**
    * Returns the power of the base of the group modulus.
    */
   Level power() const
   { return m_power; }

   /**
    * Returns the cardinality of the full group.
    */
   size_type fullSize() const
   { return power()==0 ? 1 : intPow(2,deg(base()))-1; }

   /**
    * Returns the cardinality of the group part specified by \c COMPRESS.
    * only Compress::NONE is supported in the polynomial case. 
    */
   size_type size() const
   { return fullSize(); }

   /**
    * Returns the modulus \f$b^m\f$ of the cyclic group.
    */
   Modulus modulus() const
   { return m_modulus; }

   /**
    * Returns the generator \f$g\f$ for the group.
    */
   value_type generator() const
   { return m_gen; }

   /**
    * Returns the element at index \c i.
    */
   value_type operator[](size_type i) const;

   /**
    * Returns the group generated by the inverse generator.
    */
   CyclicGroup<LatticeType::POLYNOMIAL, COMPRESS, TRAV, !ORDER> inverse() const
   { return CyclicGroup<LatticeType::POLYNOMIAL,COMPRESS, TRAV, !ORDER>(*this); }

   /**
    * Returns the subgroup at level \c level.
    */
   CyclicGroup subgroup(Level level) const
   { return CyclicGroup(*this, level); }

   /**
    * Returns the smallest generator for the group of cyclic polynomials modulo
    * \f$b^m\f$.
    * \param base    \f$b\f$
    * \param power   \f$m\f$
    * \param checkPrime    If \c true, checks if the base is actually prime.
    *
    * \remark Recall that  \f$m\f$ must be set to 1.
    */
   static value_type smallestGenerator(Modulus base, Level power, bool checkPrime = true);


private:
   template <LatticeType, LatBuilder::Compress, class, GroupOrder> friend class CyclicGroup;

   /**
    * Constructs a subgroup of \c group for level \c level, using the same generator.
    */
   CyclicGroup(const CyclicGroup& group, Level level);

   /**
    * Constructs the inverse group of \c group, using its generator to find the
    * inverse generator.
    */
   CyclicGroup(const CyclicGroup<LatticeType::POLYNOMIAL, COMPRESS, TRAV, !ORDER>& group);
   friend class CyclicGroup<LatticeType::POLYNOMIAL, COMPRESS, TRAV, !ORDER>;


   Modulus m_base;
   Level m_power;
   Modulus m_modulus;
   value_type m_gen;
};

}} // namespace

//================================================================================
// Implementation
//================================================================================

#include "latbuilder/Util.h"
// #include "latticetester/IntFactor.h"
#include <NTL/GF2XFactoring.h>

namespace LatBuilder { namespace GenSeq {

//================================================================================

template < Compress COMPRESS, class TRAV, GroupOrder ORDER>
typename CyclicGroup<LatticeType::POLYNOMIAL,COMPRESS, TRAV, ORDER>::value_type 
CyclicGroup<LatticeType::POLYNOMIAL,COMPRESS, TRAV, ORDER>::smallestGenerator(Modulus base, Level power, bool checkPrime)
{
   if (IsZero(base))
      throw std::invalid_argument("smallestGenerator(): base must be non zero");

   if (checkPrime and !IterIrredTest(base) )
      throw std::invalid_argument("smallestGenerator(): base polynomial must be irrreductible");

    if (power != 1)
      throw std::invalid_argument("smallestGenerator(): the modulus polynomial must be irreductible");

    size_type group_size = intPow(2,deg(base))-1;

    if(group_size < 2) 
        return PolynomialFromInt(1);
    
    auto factors = primeFactors(group_size);

   uInteger g = 1;
   
   while (++g <= group_size) {
      auto factor = factors.begin();
      value_type g_valueType = PolynomialFromInt(g);
      while (factor != factors.end() and
            !IsOne(modularPow(g_valueType, group_size / *factor, base)))
         ++factor;
      if (factor == factors.end())
         break;
   }
   if (g > group_size)
      throw std::logic_error("smallestGenerator(): cannot find primitive root");

   
   return PolynomialFromInt(g);
}

//================================================================================

template < Compress COMPRESS, class TRAV, GroupOrder ORDER>
CyclicGroup<LatticeType::POLYNOMIAL, COMPRESS, TRAV, ORDER>::CyclicGroup(const CyclicGroup<LatticeType::POLYNOMIAL, COMPRESS, TRAV, ORDER>& other, Level level):
   TraversalPolicy(static_cast<const TraversalPolicy&>(other)),
   m_base(other.m_base),
   m_power(level)
{
   if (m_power > other.m_power)
      throw std::invalid_argument("subgroup level is higher than group level");

   m_modulus = intPow(m_base, m_power);
   m_gen = other.m_gen;
}

//================================================================================

template <Compress COMPRESS, class TRAV, GroupOrder ORDER>
CyclicGroup<LatticeType::POLYNOMIAL, COMPRESS, TRAV, ORDER>::CyclicGroup(const CyclicGroup<LatticeType::POLYNOMIAL, COMPRESS, TRAV, !ORDER>& other):
   TraversalPolicy(static_cast<const TraversalPolicy&>(other)),
   m_base(other.m_base),
   m_power(other.m_power),
   m_modulus(other.m_modulus),
   m_gen(other.m_gen)
{
  
   m_gen = modularPow(m_gen, fullSize() - 1, modulus());
}

//================================================================================

template <Compress COMPRESS, class TRAV, GroupOrder ORDER>
CyclicGroup<LatticeType::POLYNOMIAL, COMPRESS, TRAV, ORDER>::CyclicGroup(Modulus base, Level power, Traversal trav):
   TraversalPolicy(std::move(trav)),
   m_base(base),
   m_power(power)
{
   m_modulus = intPow(m_base, m_power);
  
   
    m_gen = smallestGenerator(m_base, m_power);
    if (ORDER == GroupOrder::INVERSE)
       m_gen = modularPow(m_gen, fullSize() - 1, modulus());
   
   
}

//================================================================================

template <Compress COMPRESS, class TRAV, GroupOrder ORDER>
auto CyclicGroup<LatticeType::POLYNOMIAL, COMPRESS, TRAV, ORDER>::operator[](size_type i) const -> value_type
{
   

   const auto m = modulus();
   auto k = modularPow(generator(), i, m);

   return Compress::compressIndex(k, m);
}

}} // namespace

#endif