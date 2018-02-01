import numpy as np
import callhorizons as ch
import csv

start_date = '2017-07-10'
end_date = '2017-07-15'
time_step = '1d'
path = "../physical_properties/properties.csv"


def data_to_table(data):
    table = np.empty((0, 6), float)
    for c in data:
        x = c[2]
        y = c[3]
        z = c[4]
        vx = c[5]
        vy = c[6]
        vz = c[7]
        table = np.append(table, np.array([[x, y, z, vx, vy, vz]]), axis=0)
    return table

def save_table(table, path):
    np.savetxt(path, table, delimiter=',')

def run(path_to_config_file, from_date, to_date, time_step):

    path = "data/"
    coord_center = "500@0"
    with open(path_to_config_file) as csvfile:
        reader = csv.DictReader(csvfile, skipinitialspace=True)
        for row in reader:
            if row['ID'] == 'NA':
                body = ch.query(str(row['NAME']), smallbody=True)
                body.set_epochrange(from_date, to_date, time_step)
                body.get_vectors(coord_center)

                table = data_to_table(body.data)
                save_table(table, path + str(row['NAME']).lower() + ".csv")

            else:
                body = ch.query(str(row['ID']), smallbody=False)
                body.set_epochrange(from_date, to_date, time_step)
                body.get_vectors(coord_center)

                table = data_to_table(body.data)
                save_table(table, path + str(row['NAME']).lower() + ".csv")


run(path,start_date, end_date, time_step)