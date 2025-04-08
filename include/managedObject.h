#ifndef MANAGEDOBJECT_H
#define MANAGEDOBJECT_H

#include <memory>

template <typename ObjectType>
class ManagedObject {
    ObjectType* m_object;

public:
    explicit ManagedObject(ObjectType* obj = nullptr) noexcept : m_object(obj) {}

    ManagedObject(ManagedObject&& other) noexcept : m_object(other.Extract()) {}

    ManagedObject(const ManagedObject&) = delete;
    ManagedObject& operator=(const ManagedObject&) = delete;

    ~ManagedObject() { SafeRelease(); }

    ManagedObject& operator=(ManagedObject&& other) noexcept {
        SafeRelease(other.Extract());
        return *this;
    }

    ObjectType* operator->() const noexcept { return m_object; }

    ObjectType& operator*() const noexcept { return *m_object; }

    explicit operator bool() const noexcept { return m_object != nullptr; }

    void SafeRelease(ObjectType* newObject = nullptr) noexcept {
        delete m_object;
        m_object = newObject;
    }

    ObjectType* Extract() noexcept {
        ObjectType* temp = m_object;
        m_object = nullptr;
        return temp;
    }

    void Exchange(ManagedObject& other) noexcept {
        using std::swap;
        swap(m_object, other.m_object);
    }

    ObjectType* Raw() const noexcept { return m_object; }
};

template <typename ObjectType>
class ManagedObject<ObjectType[]> {
    ObjectType* m_object;

public:
    explicit ManagedObject(ObjectType* obj = nullptr) noexcept : m_object(obj) {}

    ~ManagedObject() { delete[] m_object; }

    ManagedObject(const ManagedObject&) = delete;
    ManagedObject& operator=(const ManagedObject&) = delete;

    ManagedObject(ManagedObject&& other) noexcept : m_object(other.Extract()) {}

    ManagedObject& operator=(ManagedObject&& other) noexcept {
        SafeRelease(other.Extract());
        return *this;
    }

    ObjectType& operator[](size_t index) const { return m_object[index]; }

    void SafeRelease(ObjectType* newObject = nullptr) noexcept {
        delete[] m_object;
        m_object = newObject;
    }

    ObjectType* Extract() noexcept {
        ObjectType* temp = m_object;
        m_object = nullptr;
        return temp;
    }
};

template <typename ObjectType, typename... Args>
ManagedObject<ObjectType> CreateManaged(Args&&... args) {
    return ManagedObject<ObjectType>(new ObjectType(std::forward<Args>(args)...));
}

#endif // MANAGEDOBJECT_H
