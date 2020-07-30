/**
 * @file IFilter.h
 * @author Jan Wielgus
 * @brief Common interface for filter libraries
 * @date 2020-07-30
 * 
 */

#ifndef IFILTER_H
#define IFILTER_H


template <class T>
class IFilter
{
public:
    virtual ~IFilter() {}

    virtual T update(T newValue) = 0;
    virtual T getFilteredValue() = 0;
    virtual void reset() = 0;
};


#endif
