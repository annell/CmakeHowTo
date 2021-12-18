//
// Created by Stefan Annell on 2021-12-16.
//

#pragma once

#include <yaml-cpp/yaml.h>
#include <string>

struct Yaml {
  YAML::Node encode() const;
  std::string name = "";
  YAML::Node node;
};

struct Bammel {
  std::string toString() const;
  std::string name = "name";
};