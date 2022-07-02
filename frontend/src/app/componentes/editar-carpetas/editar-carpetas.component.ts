import { Component, OnInit } from '@angular/core';
import { FormControl } from '@angular/forms';
import { carpetavar } from 'src/app/apiURL/baseURL';
import { CarpetaPropietario } from 'src/app/models/carpeta-propietario';
import { UsuarioService } from 'src/app/services/usuario.service';

@Component({
  selector: 'app-editar-carpetas',
  templateUrl: './editar-carpetas.component.html',
  styleUrls: ['./editar-carpetas.component.css']
})
export class EditarCarpetasComponent implements OnInit {
  Nombre = new FormControl('')
  constructor(private usuarioService: UsuarioService) { }

  ngOnInit(): void {
    this.Nombre.setValue(carpetavar.nombre)
  }


  editName(){
    

    const carpeta: CarpetaPropietario = {
      nombreCarpeta: carpetavar.nombre,
      nuevoPropietario: this.Nombre.value as string,
      nivel: 0
    }
    carpetavar.nombre = this.Nombre.value as string;

    console.log(carpeta);
    this.usuarioService.postUppdateCarp(carpeta).subscribe((res: any) => {
      console.log(res)
    }, (err) => {
      console.log(err)
    })

    this.Nombre.setValue(carpetavar.nombre)
  }
  
}
