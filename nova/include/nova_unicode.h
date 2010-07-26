п»ї/*
* Unicode Conversion library
* Based on library from Unicode, Inc.
* Written by Shibalov Nikolay (Net@Worm)
*/

/*
* This lib uses slightly rewrited convert functions
* from Unicode, Inc.
*/

/*
* Copyright 2001-2004 Unicode, Inc.
*
* Disclaimer
*
* This source code is provided as is by Unicode, Inc. No claims are
* made as to fitness for any particular purpose. No warranties of any
* kind are expressed or implied. The recipient agrees to determine
* applicability of information provided. If this file has been
* purchased on magnetic or optical media from Unicode, Inc., the
* sole remedy for any claim will be exchange of defective media
* within 90 days of receipt.
*
* Limitations on Rights to Redistribute This Code
*
* Unicode, Inc. hereby grants the right to freely use the information
* supplied in this file in the creation of products supporting the
* Unicode Standard, and to make copies of this file in any form
* for internal or external distribution as long as this notice
* remains attached.
*/

/***************************************************************************
 *   Copyright (C) 2009 by Sirius										   *
 *   SiriusStarNick@yandex.ru											   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program;					                           *
 ***************************************************************************/
#pragma once

#include "nova_error.h"

namespace nova
{

namespace unicode
{
    typedef uint   utf32_char; /* at least 32 bits */
    typedef word  utf16_char; /* at least 16 bits */
    typedef byte   utf8_char;  /* typically 8 bits */
    typedef size_t   size_type;

	typedef std::basic_string< utf8_char, char_traits<utf8_char>, nova::CNovaAlloc<utf8_char> >  utf8_string;
    typedef std::basic_string< utf16_char, char_traits<utf16_char>, nova::CNovaAlloc<utf16_char> > utf16_string;
    typedef std::basic_string< utf32_char, char_traits<utf32_char>, nova::CNovaAlloc<utf32_char> > utf32_string;

    typedef std::basic_stringstream< utf8_char, char_traits<utf8_char>, nova::CNovaAlloc<utf8_char> >  utf8_stringstream;
    typedef std::basic_stringstream< utf16_char, char_traits<utf16_char>, nova::CNovaAlloc<utf16_char> > utf16_stringstream;
    typedef std::basic_stringstream< utf32_char, char_traits<utf32_char>, nova::CNovaAlloc<utf32_char> > utf32_stringstream;

} //namespace unicode

namespace unicode
{
    /*
    * Functions to work with std::basic_string
    */

    /*
    * Convert utf8 to utf16
    */
    utf16_string str_utf8_to_utf16(const utf8_string &_utf8, bool _strict = true);

    /*
    * Convert utf16 to utf8
    */
    utf8_string str_utf16_to_utf8(const utf16_string &_utf16, bool _strict = true);

    /*
    * Convert utf8 to utf32
    */
    utf32_string str_utf8_to_utf32(const utf8_string &_utf8, bool _strict = true);

    /*
    * Convert utf32 to utf8
    */
    utf8_string str_utf32_to_utf8(const utf32_string &_utf32, bool _strict = true);

    /*
    * Convert utf16 to utf32
    */
    utf32_string str_utf16_to_utf32(const utf16_string &_utf16, bool _strict = true);

    /*
    * Convert utf32 to utf16
    */
    utf16_string str_utf32_to_utf16(const utf32_string &_utf32, bool _strict = true);

    /*
    * Convert local multi-byte string to wstring
    */
    nova::nwstring local_to_wstring(const nova::nstring &_local);
    /*
    * Convert wstring to local multi-byte string
    */
	nova::nstring wstring_to_local(const nova::nwstring &_wstr);


} //namespace unicode

namespace unicode
{
    class NOVA_EXPORT utf_converter
    {
    private:
        utf32_string m_unicode;
    public:
        utf_converter();
        utf_converter(const utf_converter &_copy);
        // nova::nstring != utf8_string (unsigned/signed)
        utf_converter(const nova::nstring &_str);
        utf_converter(const utf8_string &_str);
        utf_converter(const utf16_string &_str);
        utf_converter(const utf32_string &_str);
        ~utf_converter();

        utf8_string to_utf8() const;
        utf16_string to_utf16() const;

        nova::nstring to_local() const;
        nova::nwstring to_wstring() const;

        utf32_string& get();
        const utf32_string &get() const;
    };

    namespace exceptions
    {
        /*
        *  Base error class
        */
        class base_exception : public std::exception
        {
        private:
            nova::nwstring m_what;
            size_type m_pos;
        public:
            base_exception(const nova::nwstring &_what, const size_type &_pos);
            virtual ~base_exception() throw();
            virtual const nova::nwstring &what();
            size_type get_pos() const;
        };

        /*
        *  Source exhausted error
        *  partial character in source, but hit end
        */
        class source_exhausted : public base_exception
        {
        public:
            source_exhausted(const size_type &_pos);
            virtual ~source_exhausted() throw();
        };

        /*
        *  Destination exhausted error
        *  Insuff. room in target for conversion
        */
        class destination_exhausted : public base_exception
        {
        public:
            destination_exhausted(const size_type &_pos);
            virtual ~destination_exhausted() throw();
        };

        /*
        *  Source illegal error
        *  Source sequence is illegal/malformed
        */
        class source_illegal : public base_exception
        {
        public:
            source_illegal(const size_type &_pos);
            virtual ~source_illegal() throw();
        };
    } //namespace exceptions

} //namespace unicode

}
