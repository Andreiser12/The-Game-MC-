#include "Utils.h"

tgui::String Utils::TruncateText(const tgui::String& text, unsigned maxChars)
{
    if (text.length() < maxChars)
    {
        return text;
    }

    return text.substr(0, maxChars - 1) + "...";
}
