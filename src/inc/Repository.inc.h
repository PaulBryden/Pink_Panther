
// Do not include this file, it should only be included by Repository.h

#ifndef cppdepinject_Repository_inc_included
#define cppdepinject_Repository_inc_included

namespace cppdepinject {

    namespace impl {
        struct Type;
        struct RegisteredItem;
    }

    class Repository::Registered
    {
    public:
        typedef std::map<std::string, std::shared_ptr<impl::RegisteredItem>> TByName;
        typedef std::map<impl::Type, std::shared_ptr<TByName>> TByType;

        static const std::string& BLANK();
        static const TByName& NONE();

        template <typename T>
        const TByName& all() const;
        template <typename T>
        TByName& all();
        template <typename T>
        TByName filter(std::function<bool (const std::string&)> filter);

        template <typename T>
        std::shared_ptr<T> resolveOne(const Repository& r, const TByName& forType);
        template <typename T>
        std::map< std::string, std::shared_ptr<T> > resolveEach(const Repository& r, const TByName& forType);

        template <typename T>
        void registerItem(std::shared_ptr<impl::RegisteredItem> item, const std::string& name);

    private:
        TByType registered;
    };

    namespace impl {

        struct Type
        {
            explicit Type(const type_info& t) : t(t)  {}
            bool operator<(const Type& rhs) const  {  return t.before(rhs.t)!=0;  }
            const char* name() const  {  return t.name();  }
        private:
            const type_info& t;
        };

        struct RegisteredItem
        {
            virtual ~RegisteredItem() {}

            template<typename T>
            std::shared_ptr<T> resolve(const Repository& r)
            {
                return std::static_pointer_cast<T>(doResolve(r));
            }

        protected:
            virtual std::shared_ptr<void> doResolve(const Repository& r) = 0;
        };

        struct TypeRegisteredItem : public RegisteredItem
        {
            TypeRegisteredItem(std::function<std::shared_ptr<void> (const Repository&)> creator) : creator(creator)  {}

        protected:
            virtual std::shared_ptr<void> doResolve(const Repository& r)
            {
                return creator(r);
            }
        private:
            std::function<std::shared_ptr<void> (const Repository&)> creator;
        };

        struct InstanceRegisteredItem : public RegisteredItem
        {
            InstanceRegisteredItem(std::function<std::shared_ptr<void> (const Repository&)> creator) : creator(creator), instance()  {}

        protected:
            virtual std::shared_ptr<void> doResolve(const Repository& r)
            {	//TODO thread-safety
                // create on first call and return same object for all other calls
                if (!instance)
                    instance = creator(r);
                return instance;
            }
        private:
            std::function<std::shared_ptr<void> (const Repository&)> creator;
            std::shared_ptr<void> instance;
        };


        struct RepositoryMatch_ExactName : std::unary_function<std::string, bool>
        {
            RepositoryMatch_ExactName(const std::string& expectedName) : expectedName(expectedName)  {}
            bool operator()(const std::string& name)  {  return name == expectedName;  }
        private:
            std::string expectedName;
        };

        template <typename T>
        struct RepositoryCreator_InstanceObject : std::unary_function< Repository, std::shared_ptr<T> >
    {
        RepositoryCreator_InstanceObject(std::shared_ptr<T> singleInstance) : singleInstance(singleInstance)  {}
        std::shared_ptr<T> operator()(const Repository&)
        {
            return singleInstance;
        }
        private:
        std::shared_ptr<T> singleInstance;
    };

}

Repository::Repository() : r( new Registered )
{
}

template <typename T>
inline std::shared_ptr<T> Repository::resolve() const
{
    return r->resolveOne<T>(*this, r->all<T>());
}

template <typename T>
inline std::shared_ptr<T> Repository::resolveByName(const std::string& name) const
{
    return resolve<T>(impl::RepositoryMatch_ExactName(name));
}

template <typename T>
inline std::shared_ptr<T> Repository::resolve(std::function<bool (const std::string&)> filter) const
{
return r->resolveOne<T>(*this, r->filter<T>(filter));
}

template <typename T>
inline std::map< std::string, std::shared_ptr<T> > Repository::resolveAll() const
{
    return r->resolveEach<T>(*this, r->all<T>());
}

template <typename T>
inline std::map< std::string, std::shared_ptr<T> > Repository::resolveAll(std::function<bool (const std::string&)> filter) const
{
return r->resolveEach<T>(*this, r->filter<T>(filter));
}

template <typename T>
inline void Repository::registerInstanceObject(std::shared_ptr<T> singleInstance)
{
    registerInstanceObject<T>(singleInstance, Registered::BLANK());
}

template <typename T>
inline void Repository::registerInstanceObject(std::shared_ptr<T> singleInstance, const std::string& name)
{
    registerInstance<T>(impl::RepositoryCreator_InstanceObject<T>(singleInstance), name);
}

template <typename T>
inline void Repository::registerInstance(std::function<std::shared_ptr<T> (const Repository&)> creator)
{
registerInstance<T>(creator, Registered::BLANK());
}

template <typename T>
inline void Repository::registerInstance(std::function<std::shared_ptr<T> (const Repository&)> creator, const std::string& name)
{
r->registerItem<T>(std::shared_ptr<impl::RegisteredItem>(new impl::InstanceRegisteredItem(creator)), name);
}

template <typename T>
inline void Repository::registerType(std::function<std::shared_ptr<T> (const Repository&)> creator)
{
registerType(creator, Registered::BLANK());
}

template <typename T>
inline void Repository::registerType(std::function<std::shared_ptr<T> (const Repository&)> creator, const std::string& name)
{
r->registerItem<T>(std::shared_ptr<impl::RegisteredItem>(new impl::TypeRegisteredItem(creator)), name);
}


inline const std::string& Repository::Registered::BLANK()
{
    static std::string blank;
    return blank;
}

template <typename T>
inline const Repository::Registered::TByName& Repository::Registered::all() const
{
    var p = registered.find(impl::Type(typeid(T)));
    if (p == registered.end())
        return EMPTY();
    return *p->second;
}

template <typename T>
inline Repository::Registered::TByName& Repository::Registered::all()
{
    auto p = registered.find(impl::Type(typeid(T)));
    if (p == registered.end())
        p = registered.insert(TByType::value_type(impl::Type(typeid(T)), std::shared_ptr<TByName>(new TByName))).first;
    return *p->second;
}

template <typename T>
inline Repository::Registered::TByName Repository::Registered::filter(std::function<bool (const std::string&)> filter)
{
    TByName& all( all<T>() );
    TByName filtered;
    std::copy_if(all.begin(), all.end(), std::inserter(filtered, filtered.begin()), [=] (const TByName::value_type& v) -> bool {
        return filter(v.first);
    });
    return filtered;
}

template <typename T>
inline std::shared_ptr<T> Repository::Registered::resolveOne(const Repository& r, const TByName& forType)
{
    if (forType.empty())
        throw RepositoryException("Not registered");
    if (forType.size() > 1)
        throw RepositoryException("More than one found - use resolveAll or filter");
    return forType.begin()->second->resolve<T>(r);
}

template <typename T>
inline std::map<std::string, std::shared_ptr<T>> Repository::Registered::resolveEach(const Repository& r, const TByName& forType)
{
    typedef std::map<std::string, std::shared_ptr<T>> TOut;
    TOut out;
    std::transform(forType.begin(), forType.end(), std::inserter(out, out.begin()), [&] (const TByName::value_type& v) -> TOut::value_type {
        return std::map<std::string, std::shared_ptr<T>>::value_type( v.first, v.second->resolve<T>(r) );
    });
    return out;
}

template <typename T>
inline void Repository::Registered::registerItem(std::shared_ptr<impl::RegisteredItem> item, const std::string& name)
{
    TByName& all( all<T>() );
    if (all.find(name) != all.end())
        throw RepositoryException("Already registered");
    all.insert(TByName::value_type(name, item));
}


} // end namespace cppdepinject


#endif // cppdepinject_Repository_inc_included