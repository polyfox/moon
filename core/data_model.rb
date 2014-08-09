require 'core/data_model/field'
require 'core/data_model/verbose_validator'
require 'core/data_model/soft_validator'
Moon::DataModel::Field.validator = Moon::DataModel::SoftValidator
require 'core/data_model/metal'
require 'core/data_model/base'
