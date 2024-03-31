#pragma once

#include <detail/liber_preprocessor.h>
#include <dantelion2/utility.h>

// Part of namespace DLKR and DLKRD
#include <memory/from_allocator.h>

// Forward declarations to not include Windows.h
struct _RTL_CRITICAL_SECTION;
using HANDLE = void*;

namespace from {
    // Windows Kernel runtime functionality;
    // mutexes, signals, allocators etc.
    namespace DLKR {
        // Non-copyable dummy (empty) synchronization object
        class DLDummySyncObject : public DLUT::DLNonCopyable {};

        // A wrapper around a Windows Critical Section
        // Satisfies the C++ Mutex requirement
        // Recursive, not TimedLockable
        class DLPlainLightMutex : public DLUT::DLNonCopyable {
        public:
            LIBER_CLASS_TRAITS(
                LIBER_CLASS(DLPlainLightMutex)
            );

            DLPlainLightMutex() noexcept;
            virtual ~DLPlainLightMutex() noexcept;

            void lock() noexcept;
            void unlock() noexcept;
            bool try_lock() noexcept;

            DLPlainLightMutex(DLPlainLightMutex&&) noexcept = delete;
        private:
            _RTL_CRITICAL_SECTION* critical_section() noexcept {
                return this->_dummy_section.get();
            }

            struct alignas(8) _critical_section {
                char _dummy_section_bytes[0x28];

                _RTL_CRITICAL_SECTION* get() noexcept {
                    return reinterpret_cast<_RTL_CRITICAL_SECTION*>(this);
                }
            } _dummy_section;
        };

        LIBER_ASSERTS_BEGIN(DLPlainLightMutex);
        LIBER_ASSERT_SIZE(0x30);
        LIBER_ASSERTS_END;

        // A wrapper around a Windows kernel mutex
        // Satisfies the C++ Mutex requirement
        // Recursive, not TimedLockable
        class DLPlainMutex : public DLUT::DLNonCopyable {
        public:
            LIBER_CLASS_TRAITS(
                LIBER_CLASS(DLPlainMutex)
            );

            DLPlainMutex() noexcept;
            virtual ~DLPlainMutex() noexcept;

            void lock() noexcept;
            void unlock() noexcept;
            bool try_lock() noexcept;
            
            DLPlainMutex(DLPlainMutex&&) noexcept = delete;
        private:
            HANDLE mutex_handle;
        };

        LIBER_ASSERTS_BEGIN(DLPlainMutex);
        LIBER_ASSERT_SIZE(0x10);
        LIBER_ASSERTS_END;
    }
}