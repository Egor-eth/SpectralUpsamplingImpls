#ifndef COMMON_REFL_H
#define COMMON_REFL_H

namespace spec {

    class UniqueId
    {
    public:
        using IdType = unsigned;

        UniqueId() 
            : id(last_id++) {}
        UniqueId(const UniqueId &other)
            : id(other.id) {}

        IdType get() const 
        {
            return id;
        }

        operator IdType() const
        {
            return id;
        }

        bool operator==(const UniqueId &other) const
        {
            return other.id == id;
        }

    private:
        const IdType id;
        static IdType last_id;
    };

    class ReflectiveClass
    {
    public:
        inline static const UniqueId class_id{};
        const UniqueId &id() const { return class_id; }
    };

    template<typename T>
    inline bool isa(const ReflectiveClass *obj)
    {
        return obj->id() == T::class_id;
    }

    template<typename T>
    inline bool isa(const ReflectiveClass &obj)
    {
        return obj.id() == T::class_id;
    }

    template<typename T>
    T *dyn_cast(const ReflectiveClass *obj)
    {
        if(isa<T>(*obj)) {
            return static_cast<T>(obj);
        }
        return nullptr;
    }

}

#endif