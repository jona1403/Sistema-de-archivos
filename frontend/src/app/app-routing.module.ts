import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import {LoginComponent} from './componentes/login/login.component';
import {CrearUsuarioComponent} from './componentes/crear-usuario/crear-usuario.component';
import { OlvideContraseniaComponent } from './componentes/olvide-contrasenia/olvide-contrasenia.component';
import { InicioComponent } from './componentes/inicio/inicio.component';
import { InicioAdminComponent } from './componentes/inicio-admin/inicio-admin.component';
import { DardeAltaComponent } from './componentes/darde-alta/darde-alta.component';
import { HabilitarComponent } from './componentes/habilitar/habilitar.component';
import { AdminCarpetasArchivosComponent } from './componentes/admin-carpetas-archivos/admin-carpetas-archivos.component';

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
  {
    path: 'DarDeAlta',
    component: DardeAltaComponent,
  },
  {
    path: 'Habilitar',
    component: HabilitarComponent,
  },
  {
    path: 'AdministrarCarpetasArchivos',
    component: AdminCarpetasArchivosComponent,
  },
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
