cc_library(
  name = "vec3",
  hdrs = ["vec3.h"]
)

cc_library(
  name = "ray",
  hdrs = ["ray.h"],
  deps = [
    ":vec3"
  ]
)

cc_library(
  name = "hitable",
  hdrs = ["hitable.h"],
  deps = [
    ":ray",
  ]
)

cc_library(
  name = "sphere",
  hdrs = ["sphere.h"],
  deps = [
    ":hitable"
  ]
)

cc_library(
  name = "hitable_list",
  hdrs = ["hitable_list.h"],
  deps = [
    ":hitable"
  ]
)

cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [
    ":hitable_list",
    ":sphere",
  ]
)
