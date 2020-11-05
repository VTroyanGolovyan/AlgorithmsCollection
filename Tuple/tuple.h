#include <iostream>
#include <vector>
#include <utility>
#include <cassert>
#include <functional>
#include <algorithm>
#include <string>
#include <tuple>
#include <optional>

template<typename... Args>
class Tuple;

template <>
class Tuple<> {
public:

    enum {
        size = 0
    };

    Tuple<> () {
    }

    virtual Tuple<>& operator = (const Tuple<>& other) {
        return *this;
    }

    Tuple (const Tuple<>& other) = default;

    virtual bool operator > (const Tuple <>& other) {
        return false;
    }

    virtual bool operator < (const Tuple <>& other) {
        return false;
    }
    
    void swap(Tuple<>& t) {
	}

};

template <typename Top, typename... Bottom>
class Tuple <Top, Bottom...> : public Tuple <Bottom...>{
public:

    enum {
        size = Tuple <Bottom...>::size + 1
    };

    typedef Tuple<Top, Bottom...> tuple_type;
    typedef Top type;
	
	Tuple() {
    }
    
    Tuple(const Top& top, const Bottom&... bottom) : Tuple<Bottom...>(bottom...) {
		top_ = top;
    }
	
	Tuple(Top&& top, Bottom&&... bottom) : Tuple<Bottom...>(std::forward<Bottom>(bottom)...) {
		top_ = top;
    }
    
	void swap(tuple_type& other) {
		type tmp = std::move(other.top_);
		other.top_ = std::move(top_);
		top_ = std::move(tmp);
		children_.swap(other.children_);
	}
	
    std::optional<type> top_;
    Tuple<Bottom...>& children_ = static_cast<Tuple<Bottom...>&>(*this);

    virtual bool operator < (const Tuple <Top, Bottom...>& other) {
        return top_ < other.top_ || children_ < other.children_;
    }

    virtual bool operator > (const Tuple <Top, Bottom...>& other) {
        return top_ > other.top_ || children_ > other.children_;
    }

    virtual bool operator != (const Tuple <Top, Bottom...>& other) {
        return *this < other || *this > other;
    }

    virtual bool operator == (const Tuple <Top, Bottom...>& other) {
        return !(*this < other) && !(*this > other);
    }

    virtual bool operator <= (const Tuple <Top, Bottom...>& other) {
        return !(*this > other);
    }

    virtual bool operator >= (const Tuple <Top, Bottom...>& other) {
        return !(*this < other);
    }

    virtual Tuple <Top, Bottom...>& operator = (const Tuple <Top, Bottom...>& other) {
        if (&other != this) {
            top_ = other.top_;
            children_ = other.children_;
        }
        return *this;
    }

};

template<typename T, typename U>
struct is_same {
    enum { 
		value = 0 
	};
};

template<typename T>
struct is_same<T, T> {
    enum { 
		value = 1 
	};
};

template<int Index, typename Top, typename... Bottom>
class GetByIndex {
public:
    typedef typename GetByIndex<Index - 1, Bottom...>::return_type return_type;
    static return_type get (const Tuple<Top, Bottom...>& t) {
        return GetByIndex<Index - 1, Bottom...>::get(t);
    }
	static return_type& get (Tuple<Top, Bottom...> &t) {
        return GetByIndex<Index - 1, Bottom...>::get(t);
    }
    static return_type&& get (Tuple<Top, Bottom...> &&t) {
        return GetByIndex<Index - 1, Bottom...>::get(t);
    }
};

template <typename Top, typename... Bottom>
class GetByIndex <0, Top, Bottom...> {
public:
    typedef typename Tuple<Top, Bottom...>::type return_type;
    static return_type get (const Tuple<Top, Bottom...>& t) {
        return *t.top_;
    }
    static return_type& get (Tuple<Top, Bottom...> &t) {
        return *t.top_;
    }
    static return_type&& get (Tuple<Top, Bottom...> &&t) {
        return *t.top_;
    }
};

template<int Index, typename Head, typename... Bottom>
const typename GetByIndex<Index, Head, Bottom...>::return_type& get (const Tuple<Head, Bottom...> &t) {
    return GetByIndex<Index, Head, Bottom...>::get(t);
}

template<int Index, typename Head, typename... Bottom>
typename GetByIndex<Index, Head, Bottom...>::return_type&& get (Tuple<Head, Bottom...> &&t) {
    return std::move(GetByIndex<Index, Head, Bottom...>::get(t));
}

template<int Index, typename Head, typename... Bottom>
typename GetByIndex<Index, Head, Bottom...>::return_type& get (Tuple<Head, Bottom...>& t) {
    return GetByIndex<Index, Head, Bottom...>::get(t);
}

template <typename T, bool isEnd, typename Head, typename... Bottom>
class GetIndexByType {
public:
	using tuple_t = Tuple<Bottom...>;
	enum {
        index = GetIndexByType<T, is_same<T, Head>::value == 1 || sizeof...(Bottom) == 1, Bottom...>::index
    };
};

template <typename T, typename Head, typename... Bottom>
class GetIndexByType <T, true, Head, Bottom...> {
public:
	 using tuple_t = Tuple<Head, Bottom...>;
	 enum {
        index = tuple_t::size
     };
};

template <typename T, typename Head>
class GetIndexByType <T, true, Head> {
public:
	 enum {
        index = 1 - is_same<T, Head>::value
     };
};

template<typename T, typename Head, typename... Bottom>
typename GetByIndex< Tuple<Head, Bottom...>::size - GetIndexByType<T, false, Head, Bottom...>::index - 1, Head, Bottom...>::return_type get (const Tuple<Head, Bottom...>& t) {
    return GetByIndex< Tuple<Head, Bottom...>::size - GetIndexByType<T, false, Head, Bottom...>::index - 1, Head, Bottom...>::get(t);
}

template<typename T, typename Head, typename... Bottom>
typename GetByIndex< Tuple<Head, Bottom...>::size - GetIndexByType<T, false, Head, Bottom...>::index - 1, Head, Bottom...>::return_type& get (Tuple<Head, Bottom...>& t) {
    return GetByIndex< Tuple<Head, Bottom...>::size - GetIndexByType<T, false, Head, Bottom...>::index - 1, Head, Bottom...>::get(t);
}



template <typename... Args>
Tuple<Args...> makeTuple (Args... args) {
    return Tuple<Args...> (args...);
}


template <typename Tuple, bool isEnd, int Size, int... Index>
class callHelp2 {
public:

	enum {
	   count = sizeof...(Index)
	};
	
    template <typename... Args>
    static auto call (Tuple t, Args... args) {
        return callHelp2<
                Tuple,
                Size <= sizeof...(Index) + 1,
                Size,
                Index...
                # if Size >= count
                , sizeof...(Index)
                # endif
                >::call(t, args...);
    }
};

template <typename Tuple, int Size, int... Index>
class callHelp2 <Tuple, true, Size, Index...> {
public:
    template <typename... Args>
    static auto call (Tuple t, Args... args) {
        return makeTuple(get<Index>(t)..., args...);
    }
};

template <typename Tuple1, typename Tuple2, bool isEnd, int Size, int... Index>
class callHelp {
public:
	
	enum {
	   count = sizeof...(Index)
	};
	
    static auto call (Tuple1 t1, Tuple2 t2) {
        return callHelp<
                Tuple1,
                Tuple2,
                Size <= sizeof...(Index) + 1,
                Size,
                Index...
                # if Size >= count
                , sizeof...(Index)
                # endif
                >::call(t1,t2);
    }
};

template <typename Tuple1, typename Tuple2, int Size, int... Index>
class callHelp <Tuple1, Tuple2, true, Size, Index...> {
public:
    using tuple2_t = typename std::remove_reference<Tuple1>::type;
    static auto call (Tuple1 t1, Tuple2 t2) {
        return callHelp2<Tuple1, false, tuple2_t::size, 0>::call(t1, get<Index>(t2)...);
    }
};

template <typename Tuple1, typename Tuple2>
auto concat (Tuple1 t1, Tuple2 t2) {
    using tuple1_t = typename std::remove_reference<Tuple2>::type;
    return callHelp<Tuple1, Tuple2, false, tuple1_t::size, 0>::call(t1, t2);
}

template <typename First, typename... Other>
class concatter {
public:
	static auto tupleCat(First f, Other... other) {
		return concat(f, concatter<Other...>::tupleCat(other...));
	}
};

template <typename First>
class concatter <First> {
public:
	static auto tupleCat(First f) {
		return f;
	}
};

template <typename... Args>
auto tupleCat(Args... args) {
   return concatter<Args...>::tupleCat(args...);
}

