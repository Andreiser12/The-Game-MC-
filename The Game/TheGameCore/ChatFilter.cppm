export module ChatFilter;

import <string>;
import <regex>;

export class ChatFilter
{
public:
    static std::string FilterMessage(const std::string& message)
    {
        std::string filtered{ message };

        filtered = FilterCardPatterns(filtered);
        filtered = FilterStandaloneNumbers(filtered);

        return filtered;
    }

private:

    static std::string FilterCardPatterns(const std::string& text)
    {
        std::regex pattern(
            R"(\b(am|pot|joc|have|got|play)\s+([2-9]|[1-9][0-9])\b)",
            std::regex::icase
        );
        return std::regex_replace(text, pattern, "$1 ***");
    }

    static std::string FilterStandaloneNumbers(const std::string& text)
    {
        std::regex numberPattern(R"(\b([2-9]|[1-9][0-9])\b)");
        return std::regex_replace(text, numberPattern, "***");
    }
};
