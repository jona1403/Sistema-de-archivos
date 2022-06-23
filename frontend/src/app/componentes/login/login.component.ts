import { Component, OnInit } from '@angular/core';
import { FormControl } from '@angular/forms';
import { Usuariolog } from 'src/app/models/usuariolog';
import { UsuarioService } from '../../services/usuario.service';
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
  constructor(private usuarioService: UsuarioService) { }

  ngOnInit(): void {
  }

  login(){
    const user: Usuariolog={
      usr: this.usuario.value,
      cntr: this.contrasenia.value
    }

    this.usuarioService.postLogin(user).subscribe((res:any)=>{
      this.usuario.setValue("");
      this.contrasenia.setValue("");
    },(err)=>{
      this.mostrarMensajeError = true;
    })
  }

  desactivarMensaje(){
    this.mostrarMensaje=false
    this.mostrarMensajeError=false
  }


}
