#include <string>
#include <vector>

// FUNCTION BEHAVIOUR:
// this split cut on all delimiter found.

std::vector<std::string>
split(const std::string& s, std::string delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::string::size_type it;

    for ( size_t i = 0; i <= s.size(); i++ ) {
        it = delimiter.find(s[i]);
        if ( it == std::string::npos && i < s.size() )
            token += s[i];
        else {
            if ( !token.empty() ) {
                tokens.push_back(token);
                token.clear();
            }
        }
    }
    return tokens;
}
