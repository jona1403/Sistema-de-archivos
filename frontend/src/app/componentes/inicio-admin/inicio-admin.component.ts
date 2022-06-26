import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { usuarioAdmin, logedin, baseURL } from '../../apiURL/baseURL'

@Component({
  selector: 'app-inicio-admin',
  templateUrl: './inicio-admin.component.html',
  styleUrls: ['./inicio-admin.component.css']
})
export class InicioAdminComponent implements OnInit {

  constructor(public router: Router) { }

  ngOnInit(): void {
  }

  AdminCarpetas(){
    this.router.navigateByUrl('/AdministrarCarpetasArchivos');
  }

  Habilitar(){
    this.router.navigateByUrl('/Habilitar');
  }

  Alta(){
    this.router.navigateByUrl('/DarDeAlta');
  }

  Logout(){
    //logedin = false;
    this.router.navigateByUrl('/');
  }

}
