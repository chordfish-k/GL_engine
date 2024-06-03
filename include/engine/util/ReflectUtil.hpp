#pragma once

#include <rttr/registration>

#define BEGIN_RTTR_REG(class_name) \
    inline void register_##class_name() { \

#define END_RTTR_REG(class_name) \
    } \
    static struct class_name##_RTTR_Registrar { \
        class_name##_RTTR_Registrar() { \
            register_##class_name(); \
        } \
    } class_name##_RTTR_Registrar_instance;

