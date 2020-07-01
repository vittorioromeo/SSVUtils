// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL_INTERNAL_CNVMACROS
#define SSVU_JSON_VAL_INTERNAL_CNVMACROS

/// @macro Class mixin that allows SSVJ converters to access the current class's
/// private members.
#define SSVJ_CNV_FRIEND()         \
    template <typename, typename> \
    friend struct ssvu::Json::Impl::Cnv

/// @macro Opens a namespace for user-defined converters implementation.
/// @details Must be called outside of any namespace. Semicolon must not be
/// used.
#define SSVJ_CNV_NAMESPACE() \
    namespace ssvu           \
    {                        \
    namespace Json           \
    {                        \
    namespace Impl

/// @macro Closes a namespace for user-defined converters implementation.
/// @details Must be called after `SSVU_CNV_NAMESPACE()`. Semicolon must not be
/// used.
#define SSVJ_CNV_NAMESPACE_END() \
    }                            \
    }

/// @macro Defines a simple converter template specialization to convert classes
/// that do not require special behavior.
/// @details Must be called after `SSVJ_CNV_NAMESPACE()`. Semicolon must not be
/// used.
#define SSVJ_CNV(mType, mVName, mXName)            \
    struct Cnv<mType> final : CnvImplSimple<mType> \
    {                                              \
        template <typename TV, typename TX>        \
        inline static void impl(TV mVName, TX mXName)

/// @macro End macro, required after defining a converter.
/// @details Semicolon must not be used.
#define SSVJ_CNV_END() \
    }                  \
    ;

// Serialize members to array
#define SSVJ_IMPL_SRLZ_ARR_IMPL_SEP_ARG_STEP(mIdx, mData, mArg) \
    mData.mArg VRM_PP_COMMA_IF(mIdx)
#define SSVJ_IMPL_SRLZ_ARR_IMPL_SEP_ARG(mX, ...) \
    VRM_PP_FOREACH_REVERSE(                      \
        SSVJ_IMPL_SRLZ_ARR_IMPL_SEP_ARG_STEP, mX, __VA_ARGS__)

// Serialize members to obj with same name
#define SSVJ_IMPL_SRLZ_OBJ_AUTO_IMPL_SEP_ARG_STEP(mIdx, mData, mArg) \
    VRM_PP_TOSTR(mArg), mData.mArg VRM_PP_COMMA_IF(mIdx)
#define SSVJ_IMPL_SRLZ_OBJ_AUTO_IMPL_SEP_ARG(mX, ...) \
    VRM_PP_FOREACH_REVERSE(                           \
        SSVJ_IMPL_SRLZ_OBJ_AUTO_IMPL_SEP_ARG_STEP, mX, __VA_ARGS__)

// Wrapper macro to avoid repetition
#define SSVJ_IMPL_CNV_WRAPPER(mType, mTemplateArgs, mBody)            \
    SSVJ_CNV_NAMESPACE(){template <VRM_PP_TPL_EXPLODE(mTemplateArgs)> \
        SSVJ_CNV(mType, mV, mX){VRM_PP_TPL_EXPLODE(                   \
            mBody)} SSVJ_CNV_END()} SSVJ_CNV_NAMESPACE_END()

/// @macro Serialize/deserialize the specified member `mArg` of `mX` to a JSON
/// value in `mV`.
#define SSVJ_SRLZ_VAL(mV, mX, mArg) ::ssvj::cnv(mV, mX.mArg)

/// @macro Serialize/deserialize all specified members of `mX` to a JSON array
/// in `mV`.
#define SSVJ_SRLZ_ARR(mV, mX, ...) \
    ::ssvj::cnvArr(mV, SSVJ_IMPL_SRLZ_ARR_IMPL_SEP_ARG(mX, __VA_ARGS__))

/// @macro Serialize/deserialize all specified members of `mX` to a JSON object
/// in `mV`.
/// @details The object keys are the member names.
#define SSVJ_SRLZ_OBJ_AUTO(mV, mX, ...) \
    ::ssvj::cnvObj(mV, SSVJ_IMPL_SRLZ_OBJ_AUTO_IMPL_SEP_ARG(mX, __VA_ARGS__))

/// @macro Define a JSON converter for type `mType`, that
/// serializes/deserializes the member `mArg` to a JSON value.
#define SSVJ_CNV_VAL(mType, mArg) \
    SSVJ_IMPL_CNV_WRAPPER(mType, (), (SSVJ_SRLZ_VAL(mV, mX, mArg);))

/// @macro Define a JSON converter for type `mType`, that
/// serializes/deserializes the specified members to a JSON array.
#define SSVJ_CNV_ARR(mType, ...) \
    SSVJ_IMPL_CNV_WRAPPER(mType, (), (SSVJ_SRLZ_ARR(mV, mX, __VA_ARGS__);))

/// @macro Define a templatized JSON converter for type `mType`, that
/// serializes/deserializes the specified members to a JSON array.
/// @details Used for template types. Specify template arguments in
/// `mTemplateArgs`.
#define SSVJ_CNV_ARR_TEMPLATE(mTemplateArgs, mType, ...) \
    SSVJ_IMPL_CNV_WRAPPER(                               \
        mType, (mTemplateArgs), (SSVJ_SRLZ_ARR(mV, mX, __VA_ARGS__);))

/// @macro Define a JSON converter for type `mType`, that
/// serializes/deserializes the specified members to a JSON objects.
/// @details The object keys are the member names.
#define SSVJ_CNV_OBJ_AUTO(mType, ...) \
    SSVJ_IMPL_CNV_WRAPPER(mType, (), (SSVJ_SRLZ_OBJ_AUTO(mV, mX, __VA_ARGS__);))

#endif
