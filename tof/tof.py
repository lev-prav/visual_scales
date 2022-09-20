
from netrc import netrc
from arena_api.enums import PixelFormat
from arena_api.__future__.save import Writer
from arena_api.system import system
from arena_api.buffer import BufferFactory

import keyboard
import time
total_img = 100
def create_device_with_tries():
    '''
    Waits for the user to connect a device before raising
        an exception if it fails
    '''
    tries = 0
    tries_max = 6
    sleep_time_secs = 10
    devices = None
    while tries < tries_max:
        devices = system.create_device()
        if not devices:
            print(
                f'Try {tries+1} of {tries_max}: waiting for {sleep_time_secs} '
                f'secs for a device to be connected!')
            for sec_count in range(sleep_time_secs):
                time.sleep(1)
                print(f'{sec_count + 1 } seconds passed ',
                    '.' * sec_count, end='\r')
            tries += 1
        else:
            return devices
    else:
        raise Exception(f'No device found! Please connect a device and run '
                        f'the example again.')

def settings(device):
    tl_stream_nodemap = device.tl_stream_nodemap
    tl_stream_nodemap['StreamAutoNegotiatePacketSize'].value = True
    tl_stream_nodemap['StreamPacketResendEnable'].value = True
    nodemap = device.nodemap
    nodemap['PixelFormat'].value = PixelFormat.Coord3D_ABC16
    #enumentries  =  [Distance1250mmSingleFreq, Distance3000mmSingleFreq, Distance4000mmSingleFreq, Distance5000mmMultiFreq, Distance6000mmSingleFreq, Distance8300mmMultiFreq]
    nodemap['Scan3dOperatingMode'].value = 'Distance5000mmMultiFreq'

def example_entry_point():
    devices = create_device_with_tries()
    device = devices[0]

    '''
    Setup stream values
    '''
    settings(device)
    prev_time = time.time()
    writer = Writer()
    writer.pattern = f'images/image_<count>.ply'
    device.start_stream()
    print('Stream started, press ctrl to stop...')
    count = 0
    while True:
        buffer = device.get_buffer(1)
        writer.save(buffer)
        device.requeue_buffer(buffer)
        count+=1
        if keyboard.is_pressed('ctrl'):
            break
    print(f'Saved {count} frames')
    print(f'FPS - {count/(time.time() - prev_time)}')
    # Clean up
    
    device.stop_stream()
    print('Stream stoped')
    # Destroy Device
    system.destroy_device(device)
if __name__ == "__main__":
    print("Example Started\n")
    example_entry_point()
    print("\nExample Completed")    
