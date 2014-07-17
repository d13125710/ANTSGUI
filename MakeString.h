#include <iostream>
#include <sstream>

class make_string  
{  
public:  
    template <typename T>  
    make_string& operator<<( T const & datum )  
    {  
        buffer_ << datum;  
        return *this;  
    }  
    operator std::string () const  
    {  
        return buffer_.str();  
    }  
private:  
    std::ostringstream buffer_;  
};  