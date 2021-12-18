//
// Created by Stefan Annell on 2021-12-16.
//
#include "Yaml.h"
YAML::Node Yaml::encode() const {
  YAML::Node node;
  node["YamlLibb"] = "SomeData ?";
  return node;
}
std::string Bammel::toString() const {
  return name;
}
