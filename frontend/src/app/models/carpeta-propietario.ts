export class CarpetaPropietario {
    nombreCarpeta: string
    nuevoPropietario: string
    nivel: number

    constructor(_nombreCarpeta: string, _nuevoPropietario: string, _nivel:number) {

        this.nombreCarpeta = _nombreCarpeta;
        this.nuevoPropietario = _nuevoPropietario;
        this.nivel = _nivel;
}
}
