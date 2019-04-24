/*
    MakeRange(container<container>) should return the range 
    of all POD objects in container, so that the code 
    
    	const std::vector<std::vector<int>> data = {   
        	        {1, 2, 3},   
            	    {},   
                	{4, 5, 6}};   
	    for (const auto x : MakeRange(data)) {   
    	    std::cout << x << ", ";   
    	}
    
    would print: 1, 2, 3, 4, 5, 6
*/

template <typename OuterCollection>
class Range {
    using OuterIter = typename OuterCollection::const_iterator;
    using InnerIter = typename OuterCollection::value_type::const_iterator;

    OuterIter outerCur;
    OuterIter outerEnd;

    InnerIter innerCur;
    InnerIter innerEnd;

public:
    Range(const OuterIter& begin, const OuterIter& end): outerCur(begin), outerEnd(end) {
        if (outerCur == outerEnd)
            return;
        innerCur = outerCur->begin();
        innerEnd = outerCur->end();
        while (innerCur == innerEnd) {
            if (++outerCur == outerEnd)
                break;
            innerCur = outerCur->begin();
            innerEnd = outerCur->end();
        }
    }

    Range(const Range& other): outerCur(other.outerCur), outerEnd(other.outerEnd),
                               innerCur(other.innerCur), innerEnd(other.innerEnd) {}

    Range& operator ++() {
        ++innerCur;
        while (innerCur == innerEnd) {
            if (++outerCur == outerEnd)
                break;
            innerCur = outerCur->begin();
            innerEnd = outerCur->end();
        }
        return *this;
    }

    Range begin() const {
        Range r(outerCur, outerEnd);
        return r;
    }

    Range end() const {
        Range r(outerEnd, outerEnd);
        return r;
    }

    bool operator ==(const Range& other) const {
        return outerCur == other.outerCur;
    }

    bool operator !=(const Range& other) const {
        return !(*this == other);
    }

    const auto& operator*() const {
        return *innerCur;
    }
};

template <typename OuterCollection>
Range<OuterCollection> MakeRange(const OuterCollection& collection) {
    return {collection.begin(), collection.end()};
}
