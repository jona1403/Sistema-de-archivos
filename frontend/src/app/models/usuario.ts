export class Usuario {
    nombre: string | null
    apellido: string | null
    usuario: string | null
    correo: string | null
    numero: number | null
    contrasenia: string | null
    fechaNac: string | null
    fechaReg: string | null
    able: boolean
    type: number
    constructor(_nombre: string, _apellido: string, _usuario: string, _correo: string, _numero: number, _contrasenia: string, _fechanac: string, _fechaReg: string) {
        this.nombre = _nombre;
        this.apellido = _apellido;
        this.usuario = _usuario;
        this.correo = _correo;
        this.numero = _numero;
        this.contrasenia = _contrasenia;
        this.fechaNac = _fechanac;
        this.fechaReg = _fechaReg;
        this.able = false;
        this.type = 0;
    }
}
