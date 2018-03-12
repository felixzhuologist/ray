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

cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [
    ":ray",
  ]
)
