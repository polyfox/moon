#+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
#  Entity System proposal
#------------------------------------------------------------------------------
#  * Serialization interface
#    In order for a custom class to be serializable, it needs to define
#    to_h, export, import(data), self.load(data).
#
#  * Entity is just a uuid, that's used for identifying components.
#    In our case, it's wrapped into an object, with a few accessors to make
#    the syntax nicer. It stores no state whatsoever, besides being tied to
#    a particular world object (in order to do lookup).
#
#  * Component is a data bag.
#    It stores state about a specific property. We've created a mixin that
#    gets included into any component class, that provides the serialization
#    interface, plus a simple way to define "fields" -- ivars with accessors,
#    type checking and default values.
#
#  * System is the main logic processor.
#    It operates on specific component types (grouped per entity), and it
#    applies transformations to data. It can run every tick, or it can be async
#    and triggered when needed, or in response to some event.
#
#    Systems are usually modules. We've provided a System mixin, that defines
#    the import/export interface.
#
#  * World is our entity manager.
#    It ties all of the other parts together, and it contains the main component
#    tree, connecting entities to components.
#
#---- TODO --------------------------------------------------------------------
#  - ability to fetch entities by specific queries?
#  - make systems parallelized
#  - make inheritance and initialization easier for Components (no setup())
#  - type checking on variables
#  - (AR style validations?)
#+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
module Moon
  class Entity
    attr_reader :id

    def initialize(world)
      @world = world
      @id = @world.random.base64 16 # right within ruby's optimal string length
    end

    def ==(obj)
      self.id == obj.id
    end

    def add(component, options={})
      case component
      when Hash
        component.map do |k, v|
          @world.add_component(self, Component.fetch(k).new(v))
        end
      when Symbol
        @world.add_component(self, Component.fetch(component).new(options))
      else
        @world.add_component(self, component)
      end
    end

    def [](key)
      @world.get_component(self, key)
    end

    def []=(key, component)
      @world.set_component(self, key, component)
    end

    def to_h
      {
        id: @id
      }
    end

    def export
      to_h.stringify_keys
    end

    def import(data)
      @id = data["id"]
      self
    end

  end
end
