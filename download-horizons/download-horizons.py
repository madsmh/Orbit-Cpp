import numpy as np
from astroquery.jplhorizons import Horizons
import csv

start_date = '2017-07-10'
end_date = '2020-07-10'
time_step = '1h'
path = "../physical_properties/properties.csv"

data_path = "../data/"
data_extension = ".csv"

coord_center = "500@0"

dist_unit = 'km'
vel_unit = 'km/s'

def data_to_table(data):
    # Convert to proper units
    data['x'].convert_unit_to(dist_unit)
    data['y'].convert_unit_to(dist_unit)
    data['z'].convert_unit_to(dist_unit)

    data['vx'].convert_unit_to(vel_unit)
    data['vy'].convert_unit_to(vel_unit)
    data['vz'].convert_unit_to(vel_unit)

    x = data['x']
    y = data['y']
    z = data['z']

    vx = data['vx']
    vy = data['vy']
    vz = data['vz']

    return np.column_stack((x, y, z, vx, vy, vz))

def save_table(table, path):
    np.savetxt(path, table, delimiter=',', fmt="%1.16e")

def run(path_to_config_file, from_date, to_date, time_step):

    with open(path_to_config_file) as csvfile:
        reader = csv.DictReader(csvfile, skipinitialspace=True)
        for row in reader:
            if int(row['ID']) > 2000:
                body = Horizons(id_type='asteroid_name', id=str(row['ID']), location=coord_center,
                                epochs= {'start' : start_date, 'stop': end_date, 'step': time_step})
                vector = body.vectors()

                print(vector['targetname'][1])
                print(row['NAME'])

                table = data_to_table(vector)
                save_table(table, data_path + str(row['NAME']).lower().replace(" ", "_") + data_extension)

            else:
                body = Horizons(id_type='majorbody', id=str(row['ID']), location=coord_center,
                                epochs= {'start' : start_date, 'stop': end_date, 'step': time_step})
                vector = body.vectors()

                print(vector['targetname'][1])
                print(row['NAME'])

                table = data_to_table(vector)
                save_table(table, data_path + str(row['NAME']).lower().replace(" ", "_") + data_extension)


run(path,start_date, end_date, time_step)

