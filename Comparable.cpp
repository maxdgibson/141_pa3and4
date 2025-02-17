template<typename Derived>
class compare{
public:
    //destructor
    virtual ~comparable() = default;
    // comparing foo with foo
    friend bool operator!=(const Derived& lhs, const Derived& rhs){return !(lhs == rhs);}
    friend bool operator==(const Derived& lhs, const Derived& rhs){return (rhs == lhs);}
    friend bool operator>(const Derived& lhs, const Derived& rhs){return (rhs < lhs);}
    friend bool operator<=(const Derived& lhs, const Derived& rhs){return !(rhs < lhs);}
    friend bool operator<(const Derived& lhs, const Derived& rhs){return (lhs < rhs);}
    friend bool operator<=(const Derived& lhs, const Derived& rhs){return !(rhs < lhs);}

    using template typename<U>
    //comparing foo with any
    friend bool operator!=(const Derived& lhs, const U& rhs){return !(lhs == rhs;}
    friend bool operator==(const Derived& lhs, const U& rhs){return (rhs == lhs);}
    friend bool operator>(const Derived& lhs, const U& rhs){return (rhs < lhs);}
    friend bool operator<=(const Derived& lhs, const U& rhs){return !(rhs < lhs);}
    friend bool operator<(const Derived& lhs, const U& rhs){return (lhs < rhs);}
    friend bool operator<=(const Derived& lhs, const U& rhs){return !(rhs < lhs);}

    //comparing any with foo
    friend bool operator!=(const U& lhs, const Derived& rhs){return !(lhs == rhs;}
    friend bool operator==(const U& lhs, const Derived& rhs){return (rhs == lhs);}
    friend bool operator>(const U& lhs, const Derived& rhs){return (rhs < lhs);}
    friend bool operator<=(const U& lhs, const Derived& rhs){return !(rhs < lhs);}
    friend bool operator<(const U& lhs, const Derived& rhs){return (lhs < rhs);}
    friend bool operator<=(const U& lhs, const Derived& rhs){return !(rhs < lhs);}
};

//bool operator==(cost String& aStr) const {return compare(aStr) == 0;}