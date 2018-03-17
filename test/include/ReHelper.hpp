#pragma once

#include <string>

struct ReHelper {
static const std::string shortOptRe() { return "\\s*-([a-zA-Z])"; }
static const std::string longOptRe() { return "\\s*--([a-zA-Z-]+)"; }
static const std::string alternativesRe() { return "\\s*=\\s\\{([a-zA-Z\\s,-]+)\\}"; }
static const std::string nameRe() { return "\\s*=\\s<([a-zA-Z\\s,-]+)>"; }
static const std::string helpRe() { return "\\n\\s*([a-zA-Z\\s',\\n]*)"; }
};  // namespace ReHelper