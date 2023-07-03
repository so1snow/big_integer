#include <vector>
#include <iostream>
#include <limits.h>

#define OVERFLOW_LIM 300

// an implementation of bignum arithmetic with base INT_MAX
class big_integer
{
private:
    std::vector<int> digits;
    bool is_signed = false;

    friend std::ostream& operator << (std::ostream&, const big_integer&);

public:
    auto operator<=> (const big_integer&) const = default;

    big_integer (int number): 
        digits{number}
    {
        std::cout << "created from int" << '\n';
    }

    big_integer (const big_integer& other): 
        digits(other.digits)
    {
        std::cout << "created from vector" << '\n';
    }

    operator int()
    {
        return digits[0];
    }

    operator long()
    {
        return static_cast<long> (digits[0]) + static_cast<long> (OVERFLOW_LIM * digits[1]);
    }

    big_integer& operator=(const big_integer& other) 
    {
        this->digits = other.digits;
        return *this;
    }

    big_integer& operator-()
    {   
        is_signed = !is_signed;
        return *this;
    }

    big_integer& operator+=(const big_integer& other)
    {
        int8_t overflow = 0; // TODO add overflow check
    
        if (other.digits.size () <= digits.size())
        {
            for (std::size_t index = 0; index < other.digits.size (); ++index)
            {
                int term_1 = digits[index];
                int term_2 = other.digits[index];

                if (term_1 > OVERFLOW_LIM - term_2)
                {
                    digits[index] = term_1 + term_2 - OVERFLOW_LIM + overflow;
                    overflow = 1;
                }
                else 
                {
                    digits[index] += term_2 + overflow;
                    overflow = 0;
                }
            }
            if (overflow)
            {
                digits.push_back (1);
            }

        }
        return *this;
    }

    big_integer& operator-=(const big_integer& other)
    {
        if (other.digits.size () <= digits.size())
        {
            for (int index = 0; index < other.digits.size (); ++index)
            {
                digits[index] -= other.digits[index];
            }
        }
        return *this;
    }

    big_integer& operator++()
    {
        *this += 1;
        return *this;
    }

    big_integer operator++(int)
    {
        big_integer copy = *this;
        ++*this;
        return copy;
    }

    ~big_integer () 
    {
        std::cout << "destructed" << '\n';
    }
};

big_integer operator ""_bi (unsigned long long value)
{
    return big_integer(value);
}

big_integer operator+(const big_integer& a, const big_integer& b)
{
    big_integer result = a;
    result += b;
    return result;
}


std::ostream& operator<<(std::ostream& os, const big_integer& self)
{
    os << self.digits[0];

    for (int index = 1; index < self.digits.size(); ++index)
    {
        os << " + " << self.digits[index] << "*" << OVERFLOW_LIM << "^" << index;
    }
    return os << '\n';
}


int main ()
{
    big_integer n1 (200);
    big_integer n2 (250);

    n1 += n2;
    
    std::cout << n1  << " = int (" << int(n1) << ")\n = long (" << long(n1) << ")\n";

}