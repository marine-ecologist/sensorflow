import asyncio
from bleak import BleakClient, BleakScanner
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
from matplotlib.animation import FuncAnimation

# Define the UUIDs of the BLE service and characteristics
SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
PITCH_CHAR_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"
ROLL_CHAR_UUID = "c1c8f21d-2e89-4e17-8c39-23b5d72f935a"
YAW_CHAR_UUID = "e7a10bc2-5b7e-4e91-8b4b-7155b60ddc9a"

# Initialize figure
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Initialize a 3D object (cylinder) vertices
def create_cylinder(radius, height, num_segments):
    theta = np.linspace(0, 2 * np.pi, num_segments)
    x = radius * np.cos(theta)
    y = radius * np.sin(theta)
    z_bottom = np.zeros(num_segments)
    z_top = np.full(num_segments, height)
    return x, y, z_bottom, z_top

radius = 0.5 / 5  # One-fifth the original radius
height = 1 * 2    # Twice the original height
num_segments = 10
x, y, z_bottom, z_top = create_cylinder(radius, height, num_segments)

# Create the cylinder vertices and faces
vertices = np.vstack((np.stack((x, y, z_bottom), axis=-1), np.stack((x, y, z_top), axis=-1)))

# Function to generate faces of the cylinder
def generate_faces(num_segments):
    faces = []
    for i in range(num_segments):
        next_i = (i + 1) % num_segments
        faces.append([i, next_i, next_i + num_segments, i + num_segments])
    return faces

faces = generate_faces(num_segments)

# Function to find the device with the given service UUID
async def find_device():
    devices = await BleakScanner.discover()
    for device in devices:
        if device.metadata and "uuids" in device.metadata:
            if SERVICE_UUID.lower() in [uuid.lower() for uuid in device.metadata["uuids"]]:
                return device.address
    return None

async def read_tilt_data(client):
    try:
        pitch = await client.read_gatt_char(PITCH_CHAR_UUID)
        roll = await client.read_gatt_char(ROLL_CHAR_UUID)
        yaw = await client.read_gatt_char(YAW_CHAR_UUID)

        pitch_value = float(pitch.decode('utf-8'))
        roll_value = float(roll.decode('utf-8'))
        yaw_value = float(yaw.decode('utf-8'))

        return pitch_value, roll_value, yaw_value

    except Exception as e:
        print("Failed to read from the tilt sensor:", e)
        return None, None, None

async def update(client):
    pitch, roll, yaw = await read_tilt_data(client)

    if pitch is not None and roll is not None and yaw is not None:
        print(f"Pitch: {pitch}, Roll: {roll}, Yaw: {yaw}")

        # Create rotation matrices
        pitch_matrix = np.array([
            [1, 0, 0],
            [0, np.cos(np.radians(pitch)), -np.sin(np.radians(pitch))],
            [0, np.sin(np.radians(pitch)), np.cos(np.radians(pitch))]
        ])

        roll_matrix = np.array([
            [np.cos(np.radians(roll)), 0, np.sin(np.radians(roll))],
            [0, 1, 0],
            [-np.sin(np.radians(roll)), 0, np.cos(np.radians(roll))]
        ])

        yaw_matrix = np.array([
            [np.cos(np.radians(yaw)), -np.sin(np.radians(yaw)), 0],
            [np.sin(np.radians(yaw)), np.cos(np.radians(yaw)), 0],
            [0, 0, 1]
        ])

        # Apply rotation matrices
        rotation_matrix = yaw_matrix @ roll_matrix @ pitch_matrix
        rotated_vertices = vertices @ rotation_matrix.T

        # Clear the axis
        ax.cla()

        # Draw the cylinder
        for face in faces:
            poly3d = [[rotated_vertices[vertice] for vertice in face]]
            ax.add_collection3d(Poly3DCollection(poly3d, color="grey", alpha=0.6))

        # Set fixed axis limits to accommodate the cylinder dimensions
        ax.set_xlim(-2.5, 2.5)
        ax.set_ylim(-2.5, 2.5)
        ax.set_zlim(0, 2.5)

        # Fix aspect ratio
        ax.set_box_aspect([3, 3, 5])  # Aspect ratio is 3:3:5

        # Disable zoom
        ax.get_proj = lambda: np.dot(Axes3D.get_proj(ax), np.diag([1, 1, 1, 1]))

def animate_update(frame, client):
    asyncio.run(update(client))

async def main():
    device_address = await find_device()
    if device_address is None:
        print("Device with specified service UUID not found.")
        return

    async with BleakClient(device_address) as client:
        while True:
            await update(client)
            await asyncio.sleep(0.1)

def start_loop(loop):
    asyncio.set_event_loop(loop)
    loop.run_until_complete(main())

# Create a new event loop for asyncio
new_loop = asyncio.new_event_loop()

# Start the event loop in a new thread
import threading
thread = threading.Thread(target=start_loop, args=(new_loop,))
thread.start()

# Initial rendering of the cylinder
for face in faces:
    poly3d = [[vertices[vertice] for vertice in face]]
    ax.add_collection3d(Poly3DCollection(poly3d, color="grey", alpha=0.6))

ax.set_xlim(-2.5, 2.5)
ax.set_ylim(-2.5, 2.5)
ax.set_zlim(0, 2.5)
ax.set_box_aspect([3, 3, 5])  # Aspect ratio is 3:3:5
ax.get_proj = lambda: np.dot(Axes3D.get_proj(ax), np.diag([1, 1, 1, 1]))

ani = FuncAnimation(fig, lambda frame: None, frames=range(100), interval=100)
plt.show()
