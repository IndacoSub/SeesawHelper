#pragma once

#ifndef HELPERS_H
#define HELPERS_H

#include <type_traits>

#define CONCAT2(a, b) a ## b
#define CONCAT(a, b) CONCAT2(a, b)
#define TYPE(x) decltype(x)
#define MAKEGETTERFUNCNAME(x) CONCAT(Get, x)
#define MAKESETTERFUNCNAME(x) CONCAT(Set, x) 
#define GET(x) TYPE(x) MAKEGETTERFUNCNAME(x) (void) const {return this->x;}
#define SET(x) void MAKESETTERFUNCNAME(x) (TYPE(x) const& set) {this->x = set;}
#define GETSET(x) GET(x) SET(x)
#define REF(x) std::ref(x)

#endif