#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <cstddef>
#include <glm/glm.hpp>

namespace lc {
	namespace reflect {
		struct TypeDescriptor {
			std::string name;
			size_t size;

			TypeDescriptor(const char* name, size_t size) : name { name }, size { size } {}
			virtual std::string getName() const {
				return name;
			}
		};

		template <typename T>
		TypeDescriptor* getPrimitiveDescriptor();

		struct DefaultResolver {
			template <typename T>
			static char func(decltype(&T::Reflection));
			template <typename T>
			static int func(...);
			template <typename T>
			struct IsReflected {
				enum { value = (sizeof(func<T>(nullptr)) == sizeof(char)) };
			};

			template <typename T, typename std::enable_if<IsReflected<T>::value, int>::type = 0>
			static TypeDescriptor* get() {
				return &T::Reflection;
			}

			template <typename T, typename std::enable_if<!IsReflected<T>::value, int>::type = 0>
			static TypeDescriptor* get() {
				return getPrimitiveDescriptor<T>();
			}
		};

		template <typename T>
		struct TypeResolver {
			static TypeDescriptor* get() {
				return DefaultResolver::get<T>();
			}
		};

		struct TypeDescriptorStruct : TypeDescriptor {
			struct Member {
				const char* name;
				size_t offset;
				TypeDescriptor* type;
			};
			std::vector<Member> members;

			TypeDescriptorStruct(void (*init)(TypeDescriptorStruct*)) : TypeDescriptor { nullptr, 0 } {
				init(this);
			}
			TypeDescriptorStruct(const char* name, size_t size,
													 const std::initializer_list<Member>& init) :
					TypeDescriptor { nullptr, 0 }, members { init } {}
		};

#define REFLECT()                                                                                  \
	friend struct reflect::DefaultResolver;                                                          \
	static reflect::TypeDescriptorStruct Reflection;                                                 \
	static void initReflection(reflect::TypeDescriptorStruct*);
#define REFLECT_STRUCT_BEGIN(type)                                                                 \
	reflect::TypeDescriptorStruct type::Reflection { type::initReflection };                         \
	void type::initReflection(reflect::TypeDescriptorStruct* typeDesc) {                             \
		using T = type;                                                                                \
		typeDesc->name = #type;                                                                        \
		typeDesc->size = sizeof(T);                                                                    \
		typeDesc->members = {
#define REFLECT_STRUCT_MEMBER(name)                                                                \
	{ #name, offsetof(T, name), reflect::TypeResolver<decltype(T::name)>::get() },
#define REFLECT_STRUCT_END(type)                                                                   \
	}                                                                                                \
	;                                                                                                \
	}
	} // namespace reflect
} // namespace lc
