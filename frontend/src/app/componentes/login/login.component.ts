import { Component, OnInit } from '@angular/core';
import { FormControl } from '@angular/forms';
import { Usuariolog } from 'src/app/models/usuariolog';
import { UsuarioService } from '../../services/usuario.service';
import { Router, UrlSegment } from '@angular/router';
import { usuarioAdmin } from 'src/app/apiURL/baseURL';
@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {

  correo = new FormControl('');
  contrasenia = new FormControl('');
  mostrarMensaje= false;
  mostrarMensajeError = false;  
  contador = 0;
  constructor(private usuarioService: UsuarioService, public router: Router) { }

  ngOnInit(): void {
  }

  login(){

    const user: Usuariolog={
      usr: this.correo.value as string,
      cntr: this.contrasenia.value as string
    }

    if(user.usr == usuarioAdmin.correo && user.cntr == usuarioAdmin.contrasenia){
      this.correo.setValue('');
      this.contrasenia.setValue('');
      this.mostrarMensaje = true;
      this.router.navigateByUrl('/inicioAdmin');
      return;
    }

    this.usuarioService.postLogin(user).subscribe((res:any)=>{
      console.log(res);
      console.log(res);
      this.correo.setValue('');
      this.contrasenia.setValue('');
      if(user.usr == res.correo && user.cntr == res.contrasenia && res.able && res.alta){
        this.router.navigateByUrl('/inicio');
        this.mostrarMensaje = true;
      }else{
        this.mostrarMensajeError = true;
      }
      if(user.cntr != res.contrasenia){
        this.contador++;
      }
      if(this.contador == 3){
        //Aqui se da de baja
        
      }

    },(err)=>{
      this.mostrarMensajeError = true;
    })
  }

  desactivarMensaje(){
    this.mostrarMensaje=false
    this.mostrarMensajeError=false
  }


}
