import { Component, OnInit } from '@angular/core';
import { FormControl } from '@angular/forms';
import { archivovar } from 'src/app/apiURL/baseURL';

@Component({
  selector: 'app-editar-archivos',
  templateUrl: './editar-archivos.component.html',
  styleUrls: ['./editar-archivos.component.css']
})
export class EditarArchivosComponent implements OnInit {
  data: any = {
    texto: archivovar.texto,
  }
  Nombre = new FormControl('')
  Contenido = new FormControl('')
  constructor() { }

  ngOnInit(): void {
    this.Nombre.setValue(archivovar.nombre);
    this.data.texto = archivovar.texto;
  }

  editArchivo(){

  }

}
