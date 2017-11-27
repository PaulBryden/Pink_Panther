// TODO: rename files and namespace to cppdepinject
// TODO: add licence?
// TODO: 


#ifndef cppdepinject_Repository_included
#define cppdepinject_Repository_included

#include <memory>
#include <functional>
#include <map>
#include <algorithm>

namespace cppdepinject {

    class Repository
    {
    public:
        Repository();

        template <typename T>
        std::shared_ptr<T> resolve() const;
        template <typename T>
        std::shared_ptr<T> resolveByName(const std::string& name) const;
        template <typename T>
        std::shared_ptr<T> resolve(std::function<bool (const std::string&)> filter) const;
        template <typename T>
        std::map< std::string, std::shared_ptr<T> > resolveAll() const;
        template <typename T>
        std::map< std::string, std::shared_ptr<T> > resolveAll(std::function<bool (const std::string&)> filter) const;

        template <typename T>
        void registerInstanceObject(std::shared_ptr<T> singleInstance);
        template <typename T>
        void registerInstanceObject(std::shared_ptr<T> singleInstance, const std::string& name);
        template <typename T>
        void registerInstance(std::function<std::shared_ptr<T> (const Repository&)> creator);
        template <typename T>
        void registerInstance(std::function<std::shared_ptr<T> (const Repository&)> creator, const std::string& name);
        template <typename T>
        void registerType(std::function<std::shared_ptr<T> (const Repository&)> creator);
        template <typename T>
        void registerType(std::function<std::shared_ptr<T> (const Repository&)> creator, const std::string& name);

    private:
        class Registered;
        std::shared_ptr<Registered> r;

        // not copyable
        Repository(const Repository&);
        Repository& operator=(const Repository&);
    };

    class RepositoryException : public std::exception
    {
    public:
        RepositoryException(const char * message) : exception(message)  {}
    };

    template <typename T>
    struct SimpleCreator : public std::unary_function<Repository, std::shared_ptr<T>>
    {
        std::shared_ptr<T> operator()(const Repository&)
        {
            return std::shared_ptr<T>(new T);
        }
    };

    template <typename T, typename TP1>
    struct SimpleCreator_OneResolved : public std::unary_function<Repository, std::shared_ptr<T>>
    {
        std::shared_ptr<T> operator()(const Repository& r)
        {
            return std::shared_ptr<T>(new T(
                    r.resolve<TP1>()
            ));
        }
    };

    template <typename T, typename TP1, typename TP2>
    struct SimpleCreator_TwoResolved : public std::unary_function<Repository, std::shared_ptr<T>>
    {
        std::shared_ptr<T> operator()(const Repository& r)
        {
            return std::shared_ptr<T>(new T(
                    r.resolve<TP1>(),
                    r.resolve<TP2>()
            ));
        }
    };

    template <typename T, typename TP1, typename TP2, typename TP3>
    struct SimpleCreator_ThreeResolved : public std::unary_function<Repository, std::shared_ptr<T>>
    {
        std::shared_ptr<T> operator()(const Repository& r)
        {
            return std::shared_ptr<T>(new T(
                    r.resolve<TP1>(),
                    r.resolve<TP2>(),
                    r.resolve<TP3>()
            ));
        }
    };


} // end namespace cppdepinject


// include template implementation
#include "Repository.inc.h"


#endif // cppdepinject_Repository_included