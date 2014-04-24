class World
  attr_reader :random

  # @components = { ComponentClass => {entity_id => [component, ...], ...}, ...}
  def initialize
    @random = Moon::SeedRandom.new
    @components = Hash.new { |hash, key| hash[key] = {} } # subkeys always initialized to {}
    @entities = []
    @systems = []
  end

  # Entities

  def spawn # new entity
    entity = Entity.new self
    @entities << entity
    return entity
  end

  def [](*klasses) # get entities for each component and intersect
    klasses.map { |klass| @components[klass].keys }.inject(:&)
  end

  # Components

  # not to be used directly
  def add_component(entity, component)
    #sym = component.class.to_s.demodulize.downcase.to_sym # this is the sweetest thing...
    key = component.class
    (@components[key][entity] ||= []) << component
    component
  end

  def get_component(entity, component) # component is class here
    # .first is a hack? return first element, always
    # will be hard when we have more than one component
    # of the same type
    @components[component][entity].first
  end

  # Systems

  def register(system)
    @systems << system
  end

  #---

  def update(delta) # parallelize in the future
    @systems.each do |system|
      system.process(delta, self)
    end
  end

  def to_h
    {
      components: @components,
      entities: @entities,
      systems: @systems
    }
  end

  def export
    components = @components.each_with_object({}) do |d, comp_hash|
      component_klass, comps = *d
      entities = comps.each_with_object({}) do |a, hsh|
        eid, comp = *a
        hsh[eid] = comp.map { |c| c.export }
      end
      comp_hash[component_klass.to_s] = entities
    end
    {
      "random"     => @random.export,
      "components" => components,
      "systems"    => @systems.map { |sys| sys.export },
      "entities"   => @entities.map { |entity| entity.export }
    }
  end

  def import(data)
    @random = Moon::SeedRandom.load(data["random"])
    @components = data["components"].each_with_object({}) do |d, comp_hash|
      component_klass, comps = *d
      entities = comps.each_with_object({}) do |a, hsh|
        eid, comp = *a
        hsh[eid] = comp.map { |c| Component.load(c) }
      end
      comp_hash[component_klass.to_s] = entities
    end
    @entities = data["entities"].map do |d|
      Entity.new(self).import(d)
    end
    @systems = data["systems"].map do |d|
      System.load(d)
    end
    self
  end

  def self.load(data)
    new.import(data)
  end

end
