# Get GLM from GitHub
http_archive(
    NAME     github_glm
    URL      "https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip"
    SHA256   37e2a3d62ea3322e43593c34bae29f57e3e251ea89f4067506c94043769ade4c
    PATTERNS "glm/glm"
    STRIP    "glm")

# Create header-only library
add_library(github_glm INTERFACE)
target_include_directories(github_glm INTERFACE ${github_glm_ROOT})
