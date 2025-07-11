import os
import time
import datetime
import msvcrt  # Solo para Windows

def list_serial_ports():
    """Detecta puertos COM disponibles"""
    ports = []
    for i in range(1, 256):
        port_name = f'COM{i}'
        try:
            # Intento de abrir el puerto para detectar si existe
            test_port = os.open(f'\\\\.\\{port_name}', os.O_RDWR)
            os.close(test_port)
            ports.append(port_name)
        except OSError:
            continue
    return ports

def main():
    # Obtener puertos disponibles
    ports = list_serial_ports()
    if not ports:
        print("No se encontraron puertos seriales disponibles")
        return
    
    print("Puertos disponibles:")
    for i, port in enumerate(ports):
        print(f"{i+1}: {port}")
    
    try:
        choice = int(input("Seleccione el número del puerto: ")) - 1
        selected_port = ports[choice]
    except (ValueError, IndexError):
        print("Selección inválida")
        return
    
    # Configurar archivo CSV
    timestamp = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
    csv_file = f"datos_ir_{timestamp}.csv"
    
    print(f"\nCapturando datos de {selected_port}...")
    print(f"Archivo: {csv_file}")
    print("Presione ESC para detener\n")
    
    try:
        # Abrir puerto usando comandos del sistema
        with open(f'\\\\.\\{selected_port}', 'r+b', timeout=1) as ser:
            ser.baudrate = 115200
            ser.timeout = 100
            
            with open(csv_file, 'w') as f:
                # Escribir encabezado
                f.write("timestamp,izquierda,centro,derecha\n")
                
                start_time = time.time()
                while True:
                    # Leer datos
                    try:
                        line = ser.readline().decode('ascii', errors='ignore').strip()
                        if line:
                            timestamp = time.time() - start_time
                            f.write(f"{timestamp:.3f},{line}\n")
                            f.flush()
                            print(f"Datos: {line}")
                    except:
                        pass
                    
                    # Comprobar si se presionó ESC
                    if msvcrt.kbhit() and msvcrt.getch() == b'\x1b':
                        print("\nCaptura detenida por el usuario")
                        break
                    
                    time.sleep(0.01)
    except Exception as e:
        print(f"Error: {str(e)}")
    finally:
        print("Captura completada")

if __name__ == "__main__":
    main()