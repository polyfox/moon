
class Event
  attr_reader :type, :key
  attr_accessor :id # TEMP, proper initialization later

  def initialize(key, type, mods)
    @type = type
    @key = key

    @mods = mods # TODO
  end
end
