from constants import bp_names

class man:
    def __init__(self, name, clazz):
        self.name = name
        self.clazz = clazz
        self.body_parts = [body_part(i) for i in bp_names]
        self.hp, self.mn, self.agility, self.strength, self.intellect, self.speed = init_params[clazz]
        self.bag = []
        self.skills = []
        self.effects = []
        self.exp, self.level = 0, 0       
        
