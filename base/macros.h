#ifndef __MG_MACROS_H__
#define __MG_MACROS_H__

#define SAFE_DELETE(x)      \
    do                      \
    {                       \
        if ((x) != nullptr) \
        {                   \
            delete (x);     \
            (x) = nullptr;  \
        }                   \
    } while (0)

#define SAFE_DELETE_ARRAY(x) \
    do                       \
    {                        \
        if ((x) != nullptr)  \
        {                    \
            delete[] (x);    \
            (x) = nullptr;   \
        }                    \
    } while (0)

#define TO_UNDERLYING(x) static_cast<typename std::underlying_type<decltype(x)>::type>(x)

#define TO_ENUM(enumType, x) static_cast<enumType>(x)

#define PARSE_JSON_FILE(JSON_VAR, CONFIG_PATH)                           \
    using json = nlohmann::json;                                         \
    json JSON_VAR;                                                       \
    {                                                                    \
        std::ifstream file(CONFIG_PATH);                                 \
        if (!file.is_open())                                             \
        {                                                                \
            LOG_ERROR("{} open file failed", CONFIG_PATH);               \
            return false;                                                \
        }                                                                \
        try                                                              \
        {                                                                \
            file >> JSON_VAR;                                            \
        }                                                                \
        catch (const json::parse_error &e)                               \
        {                                                                \
            LOG_ERROR("{} json parse error: {}", CONFIG_PATH, e.what()); \
        }                                                                \
        file.close();                                                    \
    }

#endif //__MG_MACROS_H__