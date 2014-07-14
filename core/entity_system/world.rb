module Moon
  class World
    attr_reader :entities
    attr_reader :random

    # @components = { ComponentClass => {entity_id => [component, ...], ...}, ...}
    def initialize
      @random = Moon::SeedRandom.new
      @components = Hash.new { |hash, key| hash[key] = {} } # subkeys always initialized to {}
      @entities = []
      @systems = []
    end

    ## Entities

    def spawn # new entity
      entity = Entity.new self
      @entities << entity
      return entity
    end

    def [](*syms) # get entities for each component and intersect
      syms.map { |sym| @components[sym].keys }.inject(:&)
    end

    ## Components

    # not to be used directly
    def set_component(entity, component_sym, component)
      #(@components[component_sym][entity] ||= []) << component
      @components[component_sym][entity] = component
      component
    end

    def get_component(entity, component_sym) # component is class here
      # .first is a hack? return first element, always
      # will be hard when we have more than one component
      # of the same type
      @components[component_sym][entity]
    end

    def add_component(entity, component)
      component_sym = component.class.registered
      set_component(entity, component_sym, component)
    end

    ## Systems

    def register(system)
      system = System[system] if system.is_a?(Symbol)
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
        component_sym, comps = *d
        entities = comps.each_with_object({}) do |a, hsh|
          eid, comp = *a
          # entities are exported using their ids
          #hsh[eid.id] = comp.map { |c| c.export }
          hsh[eid.id] = comp.export
        end
        comp_hash[component_sym.to_s] = entities
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
      entity_table = {}
      @entities = data["entities"].map do |d|
        Entity.new(self).import(d)
      end
      entity_table = @entities.each_with_object({}) { |e, h| h[e.id] = e }
      @components = data["components"].each_with_object({}) do |d, comp_hash|
        component_sym, comps = *d
        entities = comps.each_with_object({}) do |a, hsh|
          eid, comp = *a
          # entities are imported from their ids and then remaped
          #hsh[entity_table[eid]] = comp.map { |c| Component.load(c) }
          hsh[entity_table[eid]] = Component.load(comp)
        end
        comp_hash[component_sym.to_sym] = entities
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
end
