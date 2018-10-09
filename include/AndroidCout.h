#ifndef ANDROIDCOUT_H_INCLUDED
#define ANDROIDCOUT_H_INCLUDED

#include <android/log.h>

class AndroidBuffer : public std::streambuf
{
private:
    int overflow(int c)
    {
        if (c == traits_type::eof())
        {
            *this->pptr() = traits_type::to_char_type(c);
            this->sbumpc();
        }
        return this->sync() ? traits_type::eof() : traits_type::not_eof(c);
    }

    int sync()
    {
        int rc = 0;
        if (this->pbase() != this->pptr())
        {
            __android_log_print(ANDROID_LOG_INFO,
                               "TRAINENGINE CONSOLE",
                               "%s",
                               std::string(this->pbase(),
                                           this->pptr() - this->pbase()).c_str());
            rc = 0;
            this->setp(buffer, buffer + bufsize - 1);
        }
        return rc;
    }
    char buffer[bufsize];

public:
    const int bufsize = 128;
    AndroidBuffer()
    {
        this->setp(buffer, buffer + bufsize - 1);
    }
};

#endif // ANDROIDCOUT_H_INCLUDED
