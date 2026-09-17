import math
import random
import xml.etree.ElementTree as ET
import xml.dom.minidom as minidom
import sys

ASTEROID_COUNT = 1000           # Número de asteroides a serem gerados na cintura de asteroides
BELT_DISTANCE = 78864 * 0.001   # Distância média da cintura de asteroides ao Sol em RT
BELT_SPREAD = 3000 * 0.001 	    # Tamanho radial da cintura
BELT_HEIGHT = 1500 * 0.001		# Espessura vertical da cintura

# Proporção de distância e raio dos planetas em relação à Terra, considerando a Terra como referência
planets = [
    {
        "name": "mercury", 
        "dist": 8918, 
        "radius": 0.3, 
        "rotation_sec": 1406.4,      
        "translation_sec": 2112.0     
    },
    {
        "name": "venus", 
        "dist": 16892, 
        "radius": 0.95, 
        "rotation_sec": 5832.0,      
        "translation_sec": 5392.8    
    },
    {
        "name": "earth", 
        "dist": 23455, 
        "radius": 1, 
        "rotation_sec": 24.0, 
        "translation_sec": 8766.0 
    },
    {
        "name": "mars", 
        "dist": 35732, 
        "radius": 0.53, 
        "rotation_sec": 24.6, 
        "translation_sec": 16488.0 
    },
    {
        "name": "jupiter", 
        "dist": 121996, 
        "radius": 11.2, 
        "rotation_sec": 9.9, 
        "translation_sec": 104256.0 
    },
    {
        "name": "saturn", 
        "dist": 223636, 
        "radius": 9.4, 
        "rotation_sec": 10.7, 
        "translation_sec": 257712.0 
    },
    {
        "name": "uranus", 
        "dist": 449623, 
        "radius": 4.0, 
        "rotation_sec": 17.2, 
        "translation_sec": 736344.0 
    },
    {
        "name": "neptune", 
        "dist": 705517, 
        "radius": 3.8, 
        "rotation_sec": 16.1, 
        "translation_sec": 1444176.0 
    }
]

# Unidades no XML: (Valor_Real_em_RT / Raio_do_Planeta_em_RT)
moons = {
    "earth": [
        {"name": "Luna", "scale": 0.272, "dist": 60.3, "rotation_sec": 655.7, "translation_sec": 655.7} 
    ],
    "mars": [
        {"name": "Phobos", "scale": 0.006, "dist": 2.77, "rotation_sec": 7.7, "translation_sec": 7.7},
        {"name": "Deimos", "scale": 0.0032, "dist": 6.91, "rotation_sec": 30.3, "translation_sec": 30.3}
    ],
    "jupiter": [
        {"name": "Io", "scale": 0.0023, "dist": 5.9, "rotation_sec": 42.5, "translation_sec": 42.5},
        {"name": "Europa", "scale": 0.0019, "dist": 9.4, "rotation_sec": 85.2, "translation_sec": 85.2},
        {"name": "Ganymede", "scale": 0.0033, "dist": 15.0, "rotation_sec": 171.7, "translation_sec": 171.7}, 
        {"name": "Callisto", "scale": 0.0031, "dist": 26.3, "rotation_sec": 400.5, "translation_sec": 400.5}, 
        {"name": "Amalthea", "scale": 0.00009, "dist": 2.5, "rotation_sec": 12.0, "translation_sec": 12.0},
        {"name": "Thebe", "scale": 0.00006, "dist": 3.1, "rotation_sec": 16.1, "translation_sec": 16.1},
        {"name": "Adrastea", "scale": 0.000009, "dist": 1.8, "rotation_sec": 7.1, "translation_sec": 7.1},
        {"name": "Metis", "scale": 0.00002, "dist": 1.8, "rotation_sec": 7.1, "translation_sec": 7.1}
    ],
    "saturn": [
        {"name": "Mimas", "scale": 0.0003, "dist": 3.1, "rotation_sec": 22.6, "translation_sec": 22.6},
        {"name": "Enceladus", "scale": 0.0004, "dist": 4.0, "rotation_sec": 32.9, "translation_sec": 32.9},
        {"name": "Tethys", "scale": 0.0009, "dist": 4.9, "rotation_sec": 45.3, "translation_sec": 45.3},
        {"name": "Dione", "scale": 0.0009, "dist": 6.3, "rotation_sec": 65.7, "translation_sec": 65.7},
        {"name": "Rhea", "scale": 0.0013, "dist": 8.7, "rotation_sec": 108.4, "translation_sec": 108.4},
        {"name": "Titan", "scale": 0.0045, "dist": 20.3, "rotation_sec": 382.7, "translation_sec": 382.7}, 
        {"name": "Hyperion", "scale": 0.0011, "dist": 24.8, "rotation_sec": 312.0, "translation_sec": 504.0}, 
        {"name": "Iapetus", "scale": 0.0013, "dist": 59.0, "rotation_sec": 1903.2, "translation_sec": 1903.2}, 
        {"name": "Phoebe", "scale": 0.0002, "dist": 214.0, "rotation_sec": 9.3, "translation_sec": 13212.0}, 
        {"name": "Janus", "scale": 0.0001, "dist": 2.5, "rotation_sec": 16.7, "translation_sec": 16.7},
        {"name": "Epimetheus", "scale": 0.00009, "dist": 2.5, "rotation_sec": 16.6, "translation_sec": 16.6}
    ],
    "uranus": [
        {"name": "Ariel", "scale": 0.0056, "dist": 7.4, "rotation_sec": 60.5, "translation_sec": 60.5},
        {"name": "Umbriel", "scale": 0.0058, "dist": 10.4, "rotation_sec": 99.5, "translation_sec": 99.5},
        {"name": "Titania", "scale": 0.0078, "dist": 17.0, "rotation_sec": 208.9, "translation_sec": 208.9},
        {"name": "Oberon", "scale": 0.0074, "dist": 22.7, "rotation_sec": 323.1, "translation_sec": 323.1},
        {"name": "Miranda", "scale": 0.0047, "dist": 5.1, "rotation_sec": 33.9, "translation_sec": 33.9},
        {"name": "Puck", "scale": 0.0007, "dist": 3.3, "rotation_sec": 18.3, "translation_sec": 18.3},
        {"name": "Sycorax", "scale": 0.0007, "dist": 474.0, "rotation_sec": 6.9, "translation_sec": 30919.2} 
    ],
    "neptune": [
        {"name": "Triton", "scale": 0.0144, "dist": 14.3, "rotation_sec": 141.0, "translation_sec": 141.0},
        {"name": "Proteus", "scale": 0.0022, "dist": 4.8, "rotation_sec": 26.9, "translation_sec": 26.9},
        {"name": "Nereid", "scale": 0.0018, "dist": 222.0, "rotation_sec": 11.5, "translation_sec": 8642.9},
        {"name": "Larissa", "scale": 0.0010, "dist": 2.9, "rotation_sec": 13.3, "translation_sec": 13.3},
        {"name": "Galatea", "scale": 0.0009, "dist": 2.5, "rotation_sec": 10.3, "translation_sec": 10.3},
        {"name": "Despina", "scale": 0.0008, "dist": 2.1, "rotation_sec": 8.0, "translation_sec": 8.0}
    ]
}

def complete_dict():
    counts = {"jupiter": 95, "saturn": 146, "uranus": 28, "neptune": 16}
    p_radii = {"jupiter": 11.2, "saturn": 9.4, "uranus": 4.0, "neptune": 3.8}
    
    for p_name in counts:
        if p_name in moons:
            r_planeta = p_radii[p_name]
            current_count = len(moons[p_name])
            total_needed = counts[p_name]
            
            for i in range(current_count + 1, total_needed + 1):
                real_dist = 200 + (i * 5)
                dist = round(real_dist / r_planeta, 4)
                
                radius_rt = 0.00005
                rotation_sec = 10 + i * 2
                translation_sec = 10 + i * 5

                moons[p_name].append({
                    "name": f"Minor_Moon_{i}", 
                    "scale": radius_rt, 
                    "dist": dist,
                    "rotation_sec": rotation_sec,
                    "translation_sec": translation_sec,
                })
                
def generate_comet_path(num_points, aphelion_dist, perihelion_dist):
    points = []
    
    # Gerar pontos ao redor da elipse
    for i in range(num_points):
        t = (i / (num_points - 1)) * 2 * math.pi
        
        # Posição na elipse centrada no Sol
        x = aphelion_dist * math.cos(t)
        z = perihelion_dist * math.sin(t)
        y = 0.3 * math.sin(t * 0.5)  # Pequena oscilação vertical
        
        points.append((round(x, 3), round(y, 3), round(z, 3)))
    
    return points

def add_comet(solar_system, sun_scale=1, planet_scale=1, dist_scale=1, time_scale=1):
    comet_group = ET.SubElement(solar_system, "group")
    comet_trans = ET.SubElement(comet_group, "transform")
    
    # # Garantir que não fica dentro do Sol (raio do Sol = 109 * sun_scale)
    sun_radius = 109 * sun_scale
    perihelion = sun_radius * 0.1 * 1.2  # 20% acima do raio do Sol
    aphelion = sun_radius * 0.1 * 2  # Distância máxima do cometa ao Sol
    
    # Gerar trajetória do cometa
    comet_points = generate_comet_path(14, aphelion, perihelion)
    comet_time = 50.0 * time_scale
    
    # Criar translate dinâmica com curva Catmull-Rom
    translate_elem = ET.SubElement(comet_trans, "translate", 
                                   time=str(comet_time), 
                                   align="true")
    
    for x, y, z in comet_points:
        ET.SubElement(translate_elem, "point", x=str(x), y=str(y), z=str(z))
    
    # Scale do cometa proporcional ao planeta_scale
    comet_scale = 0.05 * planet_scale
    ET.SubElement(comet_trans, "scale", x=str(comet_scale), y=str(comet_scale), z=str(comet_scale))
    
    # Modelo do cometa
    comet_models = ET.SubElement(comet_group, "models")
    ET.SubElement(comet_models, "model", file="bezier_10.3d")

def generate_xml(scales):

    complete_dict()
    
    sun_scale = 1
    planet_scale = 1
    dist_scale = 1
    angle_var_max = 360
    time_scale = 1
    add_comet_flag = False

    if(len(scales)>=4):
        sun_scale = float(scales[1])
        planet_scale = float(scales[2])
        dist_scale = float(scales[3])
        if (len(scales)>=5):
            angle_var_max = float(scales[4])
            if (len(scales)>=6):
                time_scale = float(scales[5])
            if (len(scales)>=7) and scales[6].lower() == "true":
                add_comet_flag = True   

    world = ET.Element("world")
    
    # Câmara -------------------------------------
    ET.SubElement(world, "window", width="2048", height="1440")
    camera = ET.SubElement(world, "camera")
    ET.SubElement(camera, "position", x="20", y="20", z="30")
    ET.SubElement(camera, "lookAt", x="0", y="0", z="0")
    ET.SubElement(camera, "up", x="0", y="1", z="0")
    ET.SubElement(camera, "projection", fov="60", near="1", far="1000")

    # Luz -------------------------------------
    lights = ET.SubElement(world, "lights")
    ET.SubElement(lights, "light", type="point", posx="0", posy="0", posz="0")

    solar_system = ET.SubElement(world, "group")
    # Sol --------------------------------------------
    sun_group = ET.SubElement(solar_system, "group")
    sun_trans = ET.SubElement(sun_group, "transform")
    s =  109 * sun_scale
    ET.SubElement(sun_trans, "scale", x=str(s), y=str(s), z=str(s))
    s_models = ET.SubElement(sun_group, "models")
    s_model = ET.SubElement(s_models, "model", file="sphere.3d")
    ET.SubElement(s_model, "texture", file="../textures/sun.jpg")
    sun_material = ET.SubElement(s_model, "color")
    ET.SubElement(sun_material, "diffuse", R="255", G="255", B="255")
    ET.SubElement(sun_material, "specular", R="0", G="0", B="0")
    ET.SubElement(sun_material, "ambient", R="0", G="0", B="0")
    ET.SubElement(sun_material, "emissive", R="255", G="255", B="255")
    ET.SubElement(sun_material, "shininess", value="0")

    for p in planets:
        group = ET.SubElement(solar_system, "group")
        trans = ET.SubElement(group, "transform")
        
        scale = planet_scale * p["radius"]
        dist = dist_scale * p["dist"] * 0.001 + s * 0.1
        angle = random.uniform(0, angle_var_max)
        
        # Orbita: fase inicial + rotacao temporal em torno do Sol
        ET.SubElement(trans, "rotate", angle=str(angle), x=str(0), y=str(1), z=str(0))
        ET.SubElement(trans, "rotate", time=str((p["translation_sec"] / 500.0)*time_scale), x=str(0), y=str(1), z=str(0))
        ET.SubElement(trans, "translate", x=str(0), y=str(0), z=str(dist))
        # Rotacao propria do planeta
        ET.SubElement(trans, "rotate", time=str(p["rotation_sec"]*time_scale), x=str(0), y=str(1), z=str(0))
        ET.SubElement(trans, "scale", x=str(scale), y=str(scale), z=str(scale))
        
        models = ET.SubElement(group, "models")
        model = ET.SubElement(models, "model", file="sphere.3d")
        ET.SubElement(model, "texture", file=f"../textures/{p['name']}.jpg")
        
        if p["name"] in moons:
            for m in moons[p["name"]]:
                moon_group =  ET.SubElement(group, "group")
                moon_trans = ET.SubElement(moon_group, "transform")


                moon_scale = m["scale"]
                moon_dist = 0.1 + m["dist"] * dist_scale * 0.001 + m["scale"] * 0.1
                moon_angle = random.uniform(0, angle_var_max)

                ET.SubElement(moon_trans, "rotate", angle=str(moon_angle), x=str(0), y=str(1), z=str(0))
                ET.SubElement(moon_trans, "rotate", time=str((m["translation_sec"] / 500.0)*time_scale), x=str(0), y=str(1), z=str(0))
                ET.SubElement(moon_trans, "translate", x=str(0), y=str(0), z=str(moon_dist))
                ET.SubElement(moon_trans, "rotate", time=str(m["rotation_sec"]*time_scale), x=str(0), y=str(1), z=str(0))
                ET.SubElement(moon_trans, "scale", x=str(moon_scale), y=str(moon_scale), z=str(moon_scale))
                moon_models = ET.SubElement(moon_group, "models")
                moon_model = ET.SubElement(moon_models, "model", file="sphere.3d")
                if m["name"] == "Luna":
                    ET.SubElement(moon_model, "texture", file=f"../textures/moon.jpg")
        
        # Anel de Saturno -------------------------------------
        if p["name"] == 'saturn':
                saturn_group =  ET.SubElement(group, "group")
                saturn_trans = ET.SubElement(saturn_group, "transform")

                ET.SubElement(saturn_trans, "scale", x=str(1.8), y=str(0.2), z=str(1.8))
                ET.SubElement(trans, "rotate", angle=str(28), x=str(1), y=str(1), z=str(0))

                saturn_models = ET.SubElement(saturn_group, "models")
                saturn_model = ET.SubElement(saturn_models, "model", file="torus.3d")
                ET.SubElement(saturn_model, "texture", file="../textures/ring.jpg")
                
    # Cintura de asteróides -------------------------------------
    asteroid_belt = ET.SubElement(solar_system, "group")

    asteroid_scale = planet_scale * 0.01
    belt_center_distance = BELT_DISTANCE * dist_scale
    belt_spread = BELT_SPREAD * dist_scale
    belt_height = BELT_HEIGHT * dist_scale

    for _ in range(ASTEROID_COUNT):
        angle = random.uniform(0, 2 * math.pi)
        radius = belt_center_distance + random.uniform(-belt_spread, belt_spread)
        height = random.uniform(-belt_height, belt_height)

        x = radius * math.cos(angle)
        z = radius * math.sin(angle)

        asteroid_group = ET.SubElement(asteroid_belt, "group")
        transform = ET.SubElement(asteroid_group, "transform")
        ET.SubElement(transform, "translate", x=str(x), y=str(height), z=str(z))
        ET.SubElement(
            transform,
            "scale",
            x=str(asteroid_scale),
            y=str(asteroid_scale),
            z=str(asteroid_scale),
        )

        models = ET.SubElement(asteroid_group, "models")
        ET.SubElement(models, "model", file="sphere.3d")
    
    # Cometa -------------------------------------------
    if add_comet_flag:
        add_comet(solar_system, sun_scale, planet_scale, dist_scale, time_scale)

    xml_str = minidom.parseString(ET.tostring(world)).toprettyxml(indent="    ")
    with open("solar_system.xml", "w") as f:
        f.write(xml_str)


if __name__ == "__main__":
    generate_xml(sys.argv)