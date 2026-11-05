#include "analyzers/hash_analyzer.h"
#include <openssl/evp.h>
#include <string>

namespace filescope
{
    bool HashAnalyzer::can_handle(std::span<const std::byte>) const noexcept 
    {
        return true;
    }

    AnalysisResult HashAnalyzer::analyze(std::span<const std::byte> data) 
    {
        AnalysisResult r;
        r.analyzer_name = "hash";

        if (data.empty()) return r;

        unsigned char md[EVP_MAX_MD_SIZE];
        unsigned int md_len = 0;

        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        if (ctx && EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) == 1 &&
            EVP_DigestUpdate(ctx, data.data(), data.size()) == 1 &&
            EVP_DigestFinal_ex(ctx, md, &md_len) == 1)
        {
            std::string hex;
            hex.reserve(md_len * 2);
            const char* digits = "0123456789abcdef";
            for (unsigned int i = 0; i < md_len; ++i) 
            {
                hex.push_back(digits[md[i] >> 4]);
                hex.push_back(digits[md[i] & 0xF]);
            }
            r.metadata["sha256"] = hex;
        }
        else 
        {
            r.warnings.push_back("HASH_FAILED");
        }

        if (ctx) EVP_MD_CTX_free(ctx);
        return r;
    }
}