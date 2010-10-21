#include "nova_stable_precompiled_headers.h"

#include "nova_unicode.h"

namespace
{
	using namespace nova::unicode;

    const int halfShift  = 10; /* used for shifting by 10 bits */

    const utf32_char halfBase = 0x0010000UL;
    const utf32_char halfMask = 0x3FFUL;

    const utf32_char UNI_REPLACEMENT_CHAR = 0x0000FFFD;
    const utf32_char UNI_MAX_BMP          = 0x0000FFFF;
    const utf32_char UNI_MAX_UTF16        = 0x0010FFFF;
    const utf32_char UNI_MAX_UTF32        = 0x7FFFFFFF;
    const utf32_char UNI_MAX_LEGAL_UTF32  = 0x0010FFFF;

    const utf32_char UNI_SUR_HIGH_START   = 0xD800;
    const utf32_char UNI_SUR_HIGH_END     = 0xDBFF;
    const utf32_char UNI_SUR_LOW_START    = 0xDC00;
    const utf32_char UNI_SUR_LOW_END      = 0xDFFF;

    /*
    * Index into the table below with the first nByte of a UTF-8 sequence to
    * get the number of trailing bytes that are supposed to follow it.
    * Note that *legal* UTF-8 values can't have 4 or 5-bytes. The table is
    * left as-is for anyone who may want to do such conversion, which was
    * allowed in earlier algorithms.
    */
    static const char trailingBytesForUTF8[256] =
    {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
    };

    /*
    * Magic values subtracted from a buffer value during utf8_char conversion.
    * This table contains as many values as there might be trailing bytes
    * in a UTF-8 sequence.
    */
    static const utf32_char offsetsFromUTF8[6] =
    {
        0x00000000UL,
        0x00003080UL,
        0x000E2080UL,
        0x03C82080UL,
        0xFA082080UL,
        0x82082080UL
    };

    /*
    * Once the bits are split out into bytes of UTF-8, this is a mask OR-ed
    * into the first nByte, depending on how many bytes follow.  There are
    * as many entries in this table as there are UTF-8 sequence types.
    * (I.e., one nByte sequence, two nByte... etc.). Remember that sequencs
    * for *legal* UTF-8 will be 4 or fewer bytes total.
    */
    static const utf8_char firstByteMark[7] =
    {
        0x00,
        0x00,
        0xC0,
        0xE0,
        0xF0,
        0xF8,
        0xFC
    };

    /*
    * Low level convert functions
    */

    bool is_legal_utf8(const utf8_char *_pData, size_type _size);

    size_type utf8_to_utf16(const utf8_char *_utf8, size_type _utf8_size, utf16_char *_utf16, size_type _utf16_size, bool _strict = true);

    size_type utf16_to_utf8(const utf16_char *_utf16, size_type _utf16_size, utf8_char *_utf8, size_type _utf8_size, bool _strict = true);

    size_type utf8_to_utf32(const utf8_char *_utf8, size_type _utf8_size, utf32_char *_utf32, size_type _utf32_size, bool _strict = true);

    size_type utf32_to_utf8(const utf32_char *_utf32, size_type _utf32_size, utf8_char *_utf8, size_type _utf8_size, bool _strict = true);

    size_type utf16_to_utf32(const utf16_char *_utf16, size_type _utf16_size, utf32_char *_utf32, size_type _utf32_size, bool _strict = true);

    size_type utf32_to_utf16(const utf32_char *_utf32, size_type _utf32_size, utf16_char *_utf16, size_type _utf16_size, bool _strict = true);
}

namespace
{
    /*
    * Low level convert functions
    */

    bool is_legal_utf8(const utf8_char *_pData, size_type _size)
    {
		nova::nInt32 check_size = trailingBytesForUTF8[*_pData] + 1;

        if(check_size != _size)
        {
            return false;
        }

        utf8_char a;
        const utf8_char *p_src = _pData + _size;

        switch(_size)
        {
        default: return false;
            /* Everything else falls through when "true"... */
        case 4: if ((a = (*--p_src)) < 0x80 || a > 0xBF) return false;
        case 3: if ((a = (*--p_src)) < 0x80 || a > 0xBF) return false;
        case 2: if ((a = (*--p_src)) > 0xBF) return false;

            switch (*_pData)
            {
                /* no fall-through in this inner switch */
            case 0xE0: if (a < 0xA0) return false; break;
            case 0xED: if (a > 0x9F) return false; break;
            case 0xF0: if (a < 0x90) return false; break;
            case 0xF4: if (a > 0x8F) return false; break;
            default:   if (a < 0x80) return false;
            }

        case 1: if (*_pData >= 0x80 && *_pData < 0xC2) return false;
        }

        if (*_pData > 0xF4)
        {
            return false;
        }

        return true;
    }

    size_type utf8_to_utf16(const utf8_char *_utf8, size_type _utf8_size, utf16_char *_utf16, size_type _utf16_size, bool _strict)
    {
        size_type utf8_cursor = 0;
        size_type utf16_cursor = 0;

        while (utf8_cursor < _utf8_size)
        {
            utf32_char ch = 0;
            unsigned short extraBytesToRead = trailingBytesForUTF8[_utf8[utf8_cursor]];
            if(utf8_cursor + extraBytesToRead >= _utf8_size)
            {
                throw exceptions::source_exhausted(utf8_cursor);
                break;
            }
            /* Do this check whether lenient or strict */
            if(!is_legal_utf8(&_utf8[utf8_cursor], extraBytesToRead + 1))
            {
                throw exceptions::source_illegal(utf8_cursor);
                break;
            }
            /*
            * The cases all fall through. See "Note A" below.
            */
            switch(extraBytesToRead)
            {
            case 5: ch += _utf8[utf8_cursor++]; ch <<= 6; /* remember, illegal UTF-8 */
            case 4: ch += _utf8[utf8_cursor++]; ch <<= 6; /* remember, illegal UTF-8 */
            case 3: ch += _utf8[utf8_cursor++]; ch <<= 6;
            case 2: ch += _utf8[utf8_cursor++]; ch <<= 6;
            case 1: ch += _utf8[utf8_cursor++]; ch <<= 6;
            case 0: ch += _utf8[utf8_cursor++];
            }
            ch -= offsetsFromUTF8[extraBytesToRead];

            if((_utf16) && (utf16_cursor >= _utf16_size))
            {
                utf8_cursor -= (extraBytesToRead + 1); /* Back up utf8_cursor pointer! */
                throw exceptions::source_illegal(utf8_cursor);
                break;
            }
            if (ch <= UNI_MAX_BMP) /* utf16_cursor is a character <= 0xFFFF */
            {
                /* UTF-16 surrogate values are illegal in UTF-32 */
                if(ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END)
                {
                    if (_strict)
                    {
                        utf8_cursor -= (extraBytesToRead + 1); /* return to the illegal value itself */
                        throw exceptions::source_illegal(utf8_cursor);
                        break;
                    }
                    else
                    {
                        if(_utf16)
                        {
                            _utf16[utf16_cursor] = UNI_REPLACEMENT_CHAR;
                        }
                        ++utf16_cursor;
                    }
                }
                else
                {
                    if(_utf16)
                    {
                        _utf16[utf16_cursor] = (utf16_char)ch; /* normal case */
                    }
                    ++utf16_cursor;
                }
            }
            else if (ch > UNI_MAX_UTF16)
            {
                if (_strict)
                {
                    utf8_cursor -= (extraBytesToRead + 1); /* return to the start */
                    throw exceptions::source_illegal(utf8_cursor);
                    break; /* Bail out; shouldn't continue */
                }
                else
                {
                    if(_utf16)
                    {
                        _utf16[utf16_cursor] = UNI_REPLACEMENT_CHAR;
                    }
                    ++utf16_cursor;
                }
            }
            else
            {
                if(_utf16)
                {
                    /* utf16_cursor is a character in range 0xFFFF - 0x10FFFF. */
                    if(utf16_cursor + 1 >= _utf16_size)
                    {
                        utf8_cursor -= (extraBytesToRead + 1); /* Back up utf8_cursor pointer! */
                        throw exceptions::destination_exhausted(utf8_cursor);
                        break;
                    }
                    ch -= halfBase;
                    _utf16[utf16_cursor] = (utf16_char)((ch >> halfShift) + UNI_SUR_HIGH_START);
                    _utf16[utf16_cursor] = (utf16_char)((ch & halfMask) + UNI_SUR_LOW_START);
                }
                utf16_cursor += 2;
            }
        }

        return utf16_cursor;
    }

    size_type utf16_to_utf8(const utf16_char *_utf16, size_type _utf16_size, utf8_char *_utf8, size_type _utf8_size, bool _strict)
    {
        size_type utf16_cursor = 0;
        size_type utf8_cursor = 0;

        while(utf16_cursor < _utf16_size)
        {
            utf32_char ch;
            unsigned short bytesToWrite = 0;
            const utf32_char byteMask = 0xBF;
            const utf32_char byteMark = 0x80;
            ch = _utf16[utf16_cursor++];
            /* If we have a surrogate pair, convert to utf32_char first. */
            if(ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END)
            {
                /* If the 16 bits following the high surrogate are in the source buffer... */
                if (utf16_cursor < _utf16_size)
                {
                    utf32_char ch2 = _utf16[utf16_cursor];
                    /* If it's a low surrogate, convert to utf32_char. */
                    if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END)
                    {
                        ch = ((ch - UNI_SUR_HIGH_START) << halfShift) + (ch2 - UNI_SUR_LOW_START) + halfBase;
                        ++utf16_cursor;
                    }
                    else if(_strict) /* it's an unpaired high surrogate */
                    {
                        --utf16_cursor; /* return to the illegal value itself */
                        throw exceptions::source_illegal(utf16_cursor);
                        break;
                    }
                }
                else /* We don't have the 16 bits following the high surrogate. */
                {
                    --utf16_cursor; /* return to the high surrogate */
                    throw exceptions::source_exhausted(utf16_cursor);
                    break;
                }
            }
            else if(_strict)
            {
                /* UTF-16 surrogate values are illegal in UTF-32 */
                if(ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END)
                {
                    --utf16_cursor; /* return to the illegal value itself */
                    throw exceptions::source_illegal(utf16_cursor);
                    break;
                }
            }
            /* Figure out how many bytes the result will require */
            if(ch < (utf32_char)0x80)
            {
                bytesToWrite = 1;
            }
            else if(ch < (utf32_char)0x800)
            {
                bytesToWrite = 2;
            }
            else if(ch < (utf32_char)0x10000)
            {
                bytesToWrite = 3;
            } else if(ch < (utf32_char)0x110000)
            {
                bytesToWrite = 4;
            }
            else
            {
                bytesToWrite = 3;
                ch = UNI_REPLACEMENT_CHAR;
            }

            utf8_cursor += bytesToWrite;
            if(_utf8)
            {
                if(utf8_cursor > _utf8_size)
                {
                    utf8_cursor -= bytesToWrite;
                    throw exceptions::destination_exhausted(utf16_cursor);
                    break;
                }
                switch(bytesToWrite)
                { /* note: everything falls through. */
                case 4: _utf8[--utf8_cursor] = (utf8_char)((ch | byteMark) & byteMask); ch >>= 6;
                case 3: _utf8[--utf8_cursor] = (utf8_char)((ch | byteMark) & byteMask); ch >>= 6;
                case 2: _utf8[--utf8_cursor] = (utf8_char)((ch | byteMark) & byteMask); ch >>= 6;
                case 1: _utf8[--utf8_cursor] = (utf8_char) (ch | firstByteMark[bytesToWrite]);
                }
                utf8_cursor += bytesToWrite;
            }
        }

        return utf8_cursor;
    }

    size_type utf8_to_utf32(const utf8_char *_utf8, size_type _utf8_size, utf32_char *_utf32, size_type _utf32_size, bool _strict)
    {
        size_type utf8_cursor = 0;
        size_type utf32_cursor = 0;

        while(utf8_cursor < _utf8_size)
        {
            utf32_char ch = 0;
            unsigned short extraBytesToRead = trailingBytesForUTF8[_utf8[utf8_cursor]];
            if(utf8_cursor + extraBytesToRead >= _utf8_size)
            {
                throw exceptions::source_exhausted(utf8_cursor);
                break;
            }
            /* Do this check whether lenient or strict */
            if(!is_legal_utf8(&_utf8[utf8_cursor], extraBytesToRead + 1))
            {
                throw exceptions::source_illegal(utf8_cursor);
                break;
            }
            /*
            * The cases all fall through. See "Note A" below.
            */
            switch(extraBytesToRead)
            {
            case 5: ch += _utf8[utf8_cursor++]; ch <<= 6;
            case 4: ch += _utf8[utf8_cursor++]; ch <<= 6;
            case 3: ch += _utf8[utf8_cursor++]; ch <<= 6;
            case 2: ch += _utf8[utf8_cursor++]; ch <<= 6;
            case 1: ch += _utf8[utf8_cursor++]; ch <<= 6;
            case 0: ch += _utf8[utf8_cursor++];
            }
            ch -= offsetsFromUTF8[extraBytesToRead];

            if((_utf32) && (utf32_cursor >= _utf32_size))
            {
                utf8_cursor -= (extraBytesToRead+1); /* Back up the source pointer! */
                throw exceptions::destination_exhausted(utf8_cursor);
                break;
            }

            if(ch <= UNI_MAX_LEGAL_UTF32)
            {
                /*
                * UTF-16 surrogate values are illegal in UTF-32, and anything
                * over Plane 17 (> 0x10FFFF) is illegal.
                */
                if(ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END)
                {
                    if(_strict)
                    {
                        utf8_cursor -= (extraBytesToRead + 1); /* return to the illegal value itself */
                        throw exceptions::source_illegal(utf8_cursor);
                        break;
                    }
                    else
                    {
                        if(_utf32)
                        {
                            _utf32[utf32_cursor] = UNI_REPLACEMENT_CHAR;
                        }
                        ++utf32_cursor;
                    }
                }
                else
                {
                    if(_utf32)
                    {
                        _utf32[utf32_cursor] = ch;
                    }
                    ++utf32_cursor;
                }
            }
            else /* i.e., ch > UNI_MAX_LEGAL_UTF32 */
            {
                if(_utf32)
                {
                    //result = sourceIllegal;
                    _utf32[utf32_cursor] = UNI_REPLACEMENT_CHAR;
                }
                ++utf32_cursor;
            }
        }

        return utf32_cursor;
    }

    size_type utf32_to_utf8(const utf32_char *_utf32, size_type _utf32_size, utf8_char *_utf8, size_type _utf8_size, bool _strict)
    {
        size_type utf32_cursor = 0;
        size_type utf8_cursor = 0;

        while(utf32_cursor < _utf32_size)
        {
            utf32_char ch;
            unsigned short bytesToWrite = 0;
            const utf32_char byteMask = 0xBF;
            const utf32_char byteMark = 0x80;
            ch = _utf32[utf32_cursor++];
            if(_strict)
            {
                /* UTF-16 surrogate values are illegal in UTF-32 */
                if(ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END)
                {
                    --utf32_cursor; /* return to the illegal value itself */
                    throw exceptions::source_illegal(utf32_cursor);
                    break;
                }
            }
            /*
            * Figure out how many bytes the result will require. Turn any
            * illegally large utf32_char things (> Plane 17) into replacement chars.
            */
            if(ch < (utf32_char)0x80)
            {
                bytesToWrite = 1;
            }
            else if(ch < (utf32_char)0x800)
            {
                bytesToWrite = 2;
            }
            else if(ch < (utf32_char)0x10000)
            {
                bytesToWrite = 3;
            }
            else if(ch <= UNI_MAX_LEGAL_UTF32)
            {
                bytesToWrite = 4;
            }
            else
            {
                bytesToWrite = 3;
                ch = UNI_REPLACEMENT_CHAR;
                //result = sourceIllegal;
            }

            utf8_cursor += bytesToWrite;

            if(_utf8)
            {
                if(utf8_cursor > _utf8_size)
                {
                    --utf32_cursor; /* Back up source pointer! */
                    utf8_cursor -= bytesToWrite;
                    throw exceptions::destination_exhausted(utf32_cursor);
                    break;
                }
                switch(bytesToWrite) /* note: everything falls through. */
                {
                case 3: _utf8[--utf8_cursor] = (utf8_char)((ch | byteMark) & byteMask); ch >>= 6;
                case 2: _utf8[--utf8_cursor] = (utf8_char)((ch | byteMark) & byteMask); ch >>= 6;
                case 1: _utf8[--utf8_cursor] = (utf8_char) (ch | firstByteMark[bytesToWrite]);
                }
                utf8_cursor += bytesToWrite;
            }
        }

        return utf8_cursor;
    }

    size_type utf16_to_utf32(const utf16_char *_utf16, size_type _utf16_size, utf32_char *_utf32, size_type _utf32_size, bool _strict)
    {
        size_type utf16_cursor = 0;
        size_type utf16_cursor_old = 0;
        size_type utf32_cursor = 0;

        utf32_char ch, ch2;
        while(utf16_cursor < _utf16_size)
        {
            utf16_cursor_old = utf16_cursor; /*  In case we have to back up because of target overflow. */
            ch = _utf16[utf16_cursor++];
            /* If we have a surrogate pair, convert to utf32_char first. */
            if(ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END)
            {
                /* If the 16 bits following the high surrogate are in the source buffer... */
                if (utf16_cursor < _utf16_size)
                {
                    ch2 = _utf16[utf16_cursor];
                    /* If it's a low surrogate, convert to utf32_char. */
                    if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END)
                    {
                        ch = ((ch - UNI_SUR_HIGH_START) << halfShift) + (ch2 - UNI_SUR_LOW_START) + halfBase;
                        ++utf16_cursor;
                    }
                    else if(_strict) /* it's an unpaired high surrogate */
                    {
                        --utf16_cursor; /* return to the illegal value itself */
                        throw exceptions::source_illegal(utf16_cursor);
                        break;
                    }
                }
                else /* We don't have the 16 bits following the high surrogate. */
                {
                    --utf16_cursor; /* return to the high surrogate */
                    throw exceptions::source_exhausted(utf16_cursor);
                    break;
                }
            }
            else if(_strict)
            {
                /* UTF-16 surrogate values are illegal in UTF-32 */
                if(ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END)
                {
                    --utf16_cursor; /* return to the illegal value itself */
                    throw exceptions::source_illegal(utf16_cursor);
                    break;
                }
            }

            if(_utf32)
            {
                if(utf32_cursor >= _utf32_size)
                {
                    throw exceptions::destination_exhausted(utf16_cursor_old);
                    break;
                }
                _utf32[utf32_cursor] = ch;
            }

            ++utf32_cursor;

        }

        return utf32_cursor;
    }

    size_type utf32_to_utf16(const utf32_char *_utf32, size_type _utf32_size, utf16_char *_utf16, size_type _utf16_size, bool _strict)
    {
        size_type utf32_cursor = 0;
        size_type utf16_cursor = 0;

        while (utf32_cursor < _utf32_size)
        {
            utf32_char ch;
            if ((_utf16) && (utf16_cursor >= _utf16_size))
            {
                throw exceptions::destination_exhausted(utf32_cursor);
                break;
            }
            ch = _utf32[utf32_cursor++];
            if (ch <= UNI_MAX_BMP) /* Target is a character <= 0xFFFF */
            {
                /* UTF-16 surrogate values are illegal in UTF-32; 0xffff or 0xfffe are both reserved values */
                if(ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END)
                {
                    if(_strict)
                    {
                        --utf32_cursor; /* return to the illegal value itself */
                        throw exceptions::source_illegal(utf32_cursor);
                        break;
                    }
                    else
                    {
                        if(_utf16)
                        {
                            _utf16[utf16_cursor] = UNI_REPLACEMENT_CHAR;
                        }
                        ++utf16_cursor;
                    }
                }
                else
                {
                    if(_utf16)
                    {
                        _utf16[utf16_cursor] = (utf16_char)ch; /* normal case */
                    }
                    ++utf16_cursor;
                }
            }
            else if(ch > UNI_MAX_LEGAL_UTF32)
            {
                if(_strict)
                {
                    //result = sourceIllegal;
                }
                else
                {
                    if(_utf16)
                    {
                        _utf16[utf16_cursor] = UNI_REPLACEMENT_CHAR;
                    }
                    ++utf16_cursor;
                }
            }
            else
            {
                /* target is a character in range 0xFFFF - 0x10FFFF. */
                if(_utf16)
                {
                    if(utf16_cursor + 1 >= _utf16_size)
                    {
                        --utf32_cursor; /* Back up source pointer! */
                        throw exceptions::destination_exhausted(utf32_cursor);
                        break;
                    }
                    ch -= halfBase;
                    _utf16[utf16_cursor] = (utf16_char)((ch >> halfShift) + UNI_SUR_HIGH_START);
                    _utf16[utf16_cursor + 1] = (utf16_char)((ch & halfMask) + UNI_SUR_LOW_START);
                }

                utf16_cursor += 2;
            }
        }

        return utf16_cursor;
    }
} //namespace

namespace unicode_helper
{

    template<class T>
    T utf32_to_wstring(const utf32_string &_str)
    {
        T tmp;
        return tmp;
    }

    template<>
    utf16_string utf32_to_wstring<utf16_string>(const utf32_string &_str)
    {
        return str_utf32_to_utf16(_str);
    }

    template<>
    utf32_string utf32_to_wstring<utf32_string>(const utf32_string &_str)
    {
        return _str;
    }

    template<class T>
    utf32_string wstring_to_utf32(const T &_str)
    {
        utf32_string temp;
        return temp;
    }

    template<>
    utf32_string wstring_to_utf32<utf16_string>(const utf16_string &_str)
    {
        return str_utf16_to_utf32(_str);
    }

    template<>
    utf32_string wstring_to_utf32<utf32_string>(const utf32_string &_str)
    {
        return _str;
    }
}

namespace nova
{

namespace unicode
{
    /*
    *  unistring class
    */

    utf_converter::utf_converter()
    {
    }

    utf_converter::utf_converter(const utf_converter &_copy)
        : m_unicode(_copy.get())
    {
    }

    utf_converter::utf_converter(const nova::nstring &_str)
        : m_unicode(unicode_helper::wstring_to_utf32(local_to_wstring(_str)))
    {
    }

    utf_converter::utf_converter(const utf8_string &_str)
        : m_unicode(str_utf8_to_utf32(_str))
    {
    }

    utf_converter::utf_converter(const utf16_string &_str)
        : m_unicode(str_utf16_to_utf32(_str))
    {
    }

    utf_converter::utf_converter(const utf32_string &_str)
        : m_unicode(_str)
    {
    }

    utf_converter::~utf_converter()
    {
    }

    utf8_string utf_converter::to_utf8() const
    {
        return str_utf32_to_utf8(m_unicode);
    }

    utf16_string utf_converter::to_utf16() const
    {
        return str_utf32_to_utf16(m_unicode);
    }

    nova::nstring utf_converter::to_local() const
    {
        return wstring_to_local(to_wstring());
    }

    nova::nwstring utf_converter::to_wstring() const
    {
        return unicode_helper::utf32_to_wstring<nova::nwstring>(m_unicode);
    }

    utf32_string& utf_converter::get()
    {
        return m_unicode;
    }

    const utf32_string &utf_converter::get() const
    {
        return m_unicode;
    }

    namespace exceptions
    {
        /*
        *  Base error class
        */

        base_exception::base_exception(const nova::nwstring &_what, const size_type &_pos)
            :   m_pos(_pos)
        {
            nova::nwstringstream ss;
            ss << _what << L" at [" << _pos << L"]";
            m_what = ss.str();
        }

        base_exception::~base_exception() throw()
        {
        }

        const nova::nwstring &base_exception::what()
        {
            return m_what;
        }

        size_type base_exception::get_pos() const
        {
            return m_pos;
        }

        /*
        *  Source exhausted error
        *  partial character in source, but hit end
        */

        source_exhausted::source_exhausted(const size_type &_pos)
            :   base_exception(L"Source exhausted", _pos)
        {
        }

        source_exhausted::~source_exhausted() throw()
        {
        }

        /*
        *  Destination exhausted error
        *  Insuff. room in target for conversion
        */

        destination_exhausted::destination_exhausted(const size_type &_pos)
            :   base_exception(L"Destination exhausted", _pos)
        {
        }

        destination_exhausted::~destination_exhausted() throw()
        {
        }

        /*
        *  Source illegal error
        *  Source sequence is illegal/malformed
        */

        source_illegal::source_illegal(const size_type &_pos)
            :   base_exception(L"Source illegal", _pos)
        {
        }

        source_illegal::~source_illegal() throw()
        {
        }

    } //namespace exceptions
} //namespace unicode
namespace unicode
{
    /*
    * Functions to work with std::basic_string
    */

    utf16_string str_utf8_to_utf16(const utf8_string &_utf8, bool _strict)
    {
        utf16_string utf16;

        try
        {
            size_type size = utf8_to_utf16(_utf8.c_str(), _utf8.size(), NULL, 0, _strict);
            if(0 != size)
            {
                utf16.resize(size);
                utf8_to_utf16(_utf8.c_str(), _utf8.size(), &utf16.at(0), utf16.size(), _strict);
            }
        }
        catch(exceptions::base_exception &/*_e*/)
        {
            return utf16_string();
        }

        return utf16;
    }

    utf8_string str_utf16_to_utf8(const utf16_string &_utf16, bool _strict)
    {
        utf8_string utf8;

        try
        {
            size_type size = utf16_to_utf8(_utf16.c_str(), _utf16.size(), NULL, 0, _strict);
            if(0 != size)
            {
                utf8.resize(size);
                utf16_to_utf8(_utf16.c_str(), _utf16.size(), &utf8.at(0), utf8.size(), _strict);
            }
        }
        catch(exceptions::base_exception &/*_e*/)
        {
            return utf8_string();
        }

        return utf8;
    }

    utf32_string str_utf8_to_utf32(const utf8_string &_utf8, bool _strict)
    {
        utf32_string utf32;

        try
        {
            size_type size = utf8_to_utf32(_utf8.c_str(), _utf8.size(), NULL, 0, _strict);
            if(0 != size)
            {
                utf32.resize(size);
                utf8_to_utf32(_utf8.c_str(), _utf8.size(), &utf32.at(0), utf32.size(), _strict);
            }
        }
        catch(exceptions::base_exception &/*_e*/)
        {
            return utf32_string();
        }

        return utf32;
    }

    utf8_string str_utf32_to_utf8(const utf32_string &_utf32, bool _strict)
    {
        utf8_string utf8;

        try
        {
            size_type size = utf32_to_utf8(_utf32.c_str(), _utf32.size(), NULL, 0, _strict);
            if(0 != size)
            {
                utf8.resize(size);
                utf32_to_utf8(_utf32.c_str(), _utf32.size(), &utf8.at(0), utf8.size(), _strict);
            }
        }
        catch(exceptions::base_exception &/*_e*/)
        {
            return utf8_string();
        }

        return utf8;
    }

    utf32_string str_utf16_to_utf32(const utf16_string &_utf16, bool _strict)
    {
        utf32_string utf32;

        try
        {
            size_type size = utf16_to_utf32(_utf16.c_str(), _utf16.size(), NULL, 0, _strict);
            if(0 != size)
            {
                utf32.resize(size);
                utf16_to_utf32(_utf16.c_str(), _utf16.size(), &utf32.at(0), utf32.size(), _strict);
            }
        }
        catch(exceptions::base_exception &/*_e*/)
        {
            return utf32_string();
        }

        return utf32;
    }

    utf16_string str_utf32_to_utf16(const utf32_string &_utf32, bool _strict)
    {
        utf16_string utf16;

        try
        {
            size_type size = utf32_to_utf16(_utf32.c_str(), _utf32.size(), NULL, 0, _strict);
            if(0 != size)
            {
                utf16.resize(size);
                utf32_to_utf16(_utf32.c_str(), _utf32.size(), &utf16.at(0), utf16.size(), _strict);
            }
        }
        catch(exceptions::base_exception &/*_e*/)
        {
            return utf16_string();
        }

        return utf16;
    }

    /*
    * System specific
    */

    nova::nwstring local_to_wstring(const nova::nstring &_local)
    {
        if(_local.empty())
        {
            return nova::nwstring();
        }

        nova::nstring old_loc(setlocale(LC_CTYPE, NULL));
        setlocale(LC_CTYPE, "");

        nova::nwstring wstr;

        nInt32 len = mbstowcs(NULL, _local.c_str(), _local.size());

        if(len > 0)
        {
            wstr.resize(len);
            mbstowcs(&wstr.at(0), _local.c_str(), _local.size());
        }

        setlocale(LC_CTYPE, old_loc.c_str());

        return wstr;
    }

    nova::nstring wstring_to_local(const nova::nwstring &_wstr)
    {
        if(_wstr.empty())
        {
            return nova::nstring();
        }

        nova::nstring old_loc(setlocale(LC_CTYPE, NULL));
        setlocale(LC_CTYPE, "");

        nova::nstring str;

        nInt32 len = wcstombs(NULL, _wstr.c_str(), _wstr.size());

        if(len > 0)
        {
            str.resize(len);
            wcstombs(&str.at(0), _wstr.c_str(), _wstr.size());
        }

        setlocale(LC_CTYPE, old_loc.c_str());

        return str;
    }
} //namespace unicode

}
