import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import {LoginComponent} from './componentes/login/login.component';
import {CrearUsuarioComponent} from './componentes/crear-usuario/crear-usuario.component';
import { OlvideContraseniaComponent } from './componentes/olvide-contrasenia/olvide-contrasenia.component';
import { InicioComponent } from './componentes/inicio/inicio.component';
import { InicioAdminComponent } from './componentes/inicio-admin/inicio-admin.component';

const routes: Routes = [
  {
    path: '',
    component: LoginComponent,
  },
  {
    path: 'CrearUsuario',
    component: CrearUsuarioComponent,
  },
  {
    path: 'OlvideMiContrasenia',
    component: OlvideContraseniaComponent,
  },
  {
    path: 'inicio',
    component: InicioComponent,
  },
  {
    path: 'inicioAdmin',
    component: InicioAdminComponent,
  },
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
