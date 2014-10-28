#undef m_generic_unwrap
#undef m_generic_unwrap_self
#undef m_generic_extract
#undef m_generic_extract_to

/*
 * Convience macro for extracting
 * @param [Identifier] _src_  mrb_value variable to extract data from
 * @param [Identifier] _dest_  target variable to extract data to
 */
#define m_generic_unwrap(__moon_struct__, __data_type__, _src_, _dest_) __moon_struct__* _dest_ = (__moon_struct__*)mrb_data_get_ptr(mrb, _src_, __data_type__)
/*
 * @param [Identifier] _dest_
 */
#define m_generic_unwrap_self(__moon_struct__, __data_type__, _dest_) m_generic_unwrap(__moon_struct__, __data_type__, self, _dest_)
/*
 * Use this function to extract the current args as a data
 * This will create a new variable _name_
 * @param [Identifier]
 */
#define m_generic_extract(__struct__, __extract_func__, _name_) __struct__ _name_ = __extract_func__(mrb)
/*
 * Use this function to extract the current args as a data unto an existing variable _name_
 * @param [Identifier]
 */
#define m_generic_extract_to(__extract_func__, _name_) _name_ = __extract_func__(mrb)

/* Current context - convinience macros */
// VECTOR_DATA_TYPE
// VECTOR_MOON_STRUCT
// VECTOR_STRUCT
// VECTOR_EXTRACT_FUNC(_mrb_)
#undef m_vector_unwrap
#undef m_vector_unwrap_self
#undef m_vector_extract
#undef m_vector_operator_head
#undef m_vector_operator
#undef m_vector_mutate_copy
/*
 * convience macro for extracting
 * @param [Identifier] _src_  mrb_value variable to extract vector from
 * @param [Identifier] _dest_  target variable to extract vector to
 */
#define m_vector_unwrap(_src_, _dest_) m_generic_unwrap(VECTOR_MOON_STRUCT, VECTOR_DATA_TYPE, _src_, _dest_)
/*
 * @param [Identifier] _dest_
 */
#define m_vector_unwrap_self(_dest_) m_generic_unwrap_self(VECTOR_MOON_STRUCT, VECTOR_DATA_TYPE, _dest_)
/*
 * Use this function to extract the current args as a vector
 * This will create a new variable _name_
 * @param [Identifier]
 */
#define m_vector_extract(_name_) m_generic_extract(VECTOR_STRUCT, VECTOR_EXTRACT_FUNC, _name_)
/*
 * Use this function to extract the current args as a vector unto an existing variable _name_
 * @param [Identifier]
 */
#define m_vector_extract_to(_name_) m_generic_extract_to(VECTOR_EXTRACT_FUNC, _name_)
/*
 * Use this macro for functions that take another vector as an argument, but doesn't return a vector
 * @param [Identifier] _name1_  name of unwrap vector VECTOR_STRUCT (self)
 * @param [Identifier] _name2_  name of extracted vector VECTOR_STRUCT (args)
 */
#define m_vector_operator_head(_name1_, _name2_)             \
  m_vector_unwrap_self(_name1_);                             \
  m_vector_extract(_name2_)
/*
 * Use this macro to define operator functions such as + - / *
 * @param _operator_
 * @eg m_vector_operator(+);
 */
#define m_vector_operator(_operator_)                                 \
  mrb_value result = mrb_obj_dup(mrb, self);                          \
  m_vector_extract(args_vec);                                         \
  **((VECTOR_MOON_STRUCT*)DATA_PTR(result)) _operator_ ## = args_vec; \
  return result
/*
 * Use this macro for functions that mutates self as a copy
 * _op_ is any statement which results in VECTOR_STRUCT
 */
#define m_vector_mutate_copy(_op_)           \
  mrb_value result = mrb_obj_dup(mrb, self); \
  m_vector_unwrap(result, result_vec);       \
  **result_vec = _op_;                       \
  return result
