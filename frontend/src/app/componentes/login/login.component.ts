import { Component, OnInit } from '@angular/core';
import { FormControl } from '@angular/forms';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {

  usuario = new FormControl('');
  contrasenia = new FormControl('');
  mostrarMensaje= false;
  mostrarMensajeError = false;  
  constructor() { }

  ngOnInit(): void {
  }

  crearEstudiante(){
    /*const estudiante: Estudiante={
      carnet:Number(this.carnet.value),
      nombres:this.nombres.value,
      apellidos:this.apellidos.value,
      cui:String(this.cui.value),
      correo:this.correo.value,
      listaCursos:[]
    }
    console.log(this.cui.value)
    this.estudianteService.postEstudiante(estudiante).subscribe((res:any)=>{
      this.mostrarMensaje=true
      this.carnet.setValue("")
      this.nombres.setValue("")
      this.apellidos.setValue("")
      this.cui.setValue("")
      this.correo.setValue("")
      console.log("Estudiante Creado")
    },(err)=>{
      this.mostrarMensajeError=true
    })*/

  }

  desactivarMensaje(){
    this.mostrarMensaje=false
    this.mostrarMensajeError=false
  }


}
