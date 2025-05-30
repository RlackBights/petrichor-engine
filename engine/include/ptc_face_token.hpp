#ifndef PTC_FACE_TOKEN_HPP
#define PTC_FACE_TOKEN_HPP

#include <cstdio>
#include <string>
#include <variant>
struct FaceToken
{
    int v = 0;
    int vt = 0;
    int vn = 0;

    FaceToken(std::string token)
    {
        if (token.find('/') == std::variant_npos) {
            sscanf(token.c_str(), "%d", &this->v);
        } else if (token.substr(token.find('/') + 1).find('/') == std::variant_npos) {
            sscanf(token.c_str(), "%d/%d", &this->v, &this->vt);
        } else if (token.find("//") != std::variant_npos) {
            sscanf(token.c_str(), "%d//%d", &this->v, &this->vn);
        } else {
            sscanf(token.c_str(), "%d/%d/%d", &this->v, &this->vt, &this->vn);
        }
    }
};

#endif