/**
 * @file Factorize.h
 * @author Arjun Earthperson
 * @date 10/06/2023
 * @brief TODO:: DOCUMENT
*/

#ifndef NE591_008_FACTORIZE_H
#define NE591_008_FACTORIZE_H

// TODO:: DOCUMENT
namespace MyFactorizationMethod {
    // TODO:: DOCUMENT
    enum Type {
        METHOD_LU, ////< LU factorization
        METHOD_LUP, ////< LU factorization with pivoting
    };

    // TODO:: DOCUMENT
    const char *TypeKey(MyFactorizationMethod::Type value) {
        static const char *methodTypeKeys[] = {
                "LU",
                "LUP",
        };
        return methodTypeKeys[static_cast<int>(value)];
    }
}
#endif //NE591_008_FACTORIZE_H
