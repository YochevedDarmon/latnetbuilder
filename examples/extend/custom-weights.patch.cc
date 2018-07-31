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

diff --git a/include/latbuilder/Parser/Weights.h b/include/latbuilder/Parser/Weights.h
--- a/include/latbuilder/Parser/Weights.h
+++ b/include/latbuilder/Parser/Weights.h
    static std::unique_ptr<LatticeTester::Weights>
   static std::unique_ptr<LatticeTester::Weights>
diff --git a/src/Parser/Weights.cc b/src/Parser/Weights.cc
--- a/src/Parser/Weights.cc
+++ b/src/Parser/Weights.cc
 #include "latticetester/OrderDependentWeights.h"
 #include "latticetester/PODWeights.h"
 #include "latticetester/ProductWeights.h"
#include "latticetester/MyWeights.h"
 #include "latticetester/Coordinates.h"
    return std::unique_ptr<LatticeTester::Weights>(w);
std::unique_ptr<LatticeTester::Weights>
   auto w = new LatticeTester::MyWeights(ka.second);
   return std::unique_ptr<LatticeTester::Weights>(w);
 std::unique_ptr<LatticeTester::Weights> 
diff --git a/latticetester/include/latticetester/MyWeights.h b/latticetester/include/latticetester/MyWeights.h
+++ b/latticetester/include/latticetester/MyWeights.h
#ifndef LATTICETESTER__MY_WEIGHTS_H
#define LATTICETESTER__MY_WEIGHTS_H
#include "latticetester/ProductWeights.h"
namespace LatticeTester {