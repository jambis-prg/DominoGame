#pragma once

#include "../Debug/InternalAssert.h"
#include "../Debug/Log.h"
#include <vector>
#include <exception>
#include <memory>
#include <string>

class DataPacket
{
private:
    std::vector<char> m_Data;
    std::size_t m_ReadPos = 0;

    void Append(const void *data, std::size_t size)
    {
        m_Data.insert(m_Data.end(), static_cast<const char *>(data), static_cast<const char *>(data) + size);
    }
public:
    DataPacket() = default;

    template<typename T>
    DataPacket &operator<<(const T& data)
    {
        ASSERT(std::is_trivially_copyable<T>::value, "Tipo nao suportado");
        Append(&data, sizeof(T));
        return *this;
    }

    template<typename T>
    DataPacket& operator>>(T& data) 
    {
        ASSERT(std::is_trivially_copyable<T>::value, "Tipo não suportado");
        if (m_ReadPos + sizeof(T) > m_Data.size()) 
            throw std::out_of_range("Tentativa de ler além dos limites do pacote");
        
        memcpy(&data, &m_Data[m_ReadPos], sizeof(T));
        m_ReadPos += sizeof(T);
        return *this;
    }

    // Especialização para strings
    DataPacket& operator<<(const std::string& data) 
    {
        uint32_t length = static_cast<uint32_t>(data.size());
        *this << length;
        Append(data.data(), length);
        return *this;
    }
    
    DataPacket& operator>>(std::string& data) 
    {
        uint32_t length = 0;
        *this >> length;
        if (m_ReadPos + length > m_Data.size())
            throw std::out_of_range("Tentativa de ler além dos limites do pacote");
        data.assign(&m_Data[m_ReadPos], length);
        m_ReadPos += length;
        return *this;
    }

    const void* GetData() const { return m_Data.data(); }
    std::size_t GetSize() const { return m_Data.size(); }
    void Clear() { m_Data.clear(); m_ReadPos = 0; }
    void ResetRead() { m_ReadPos = 0; }
};