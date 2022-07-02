import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { LoginComponent } from './componentes/login/login.component';
import { CrearUsuarioComponent } from './componentes/crear-usuario/crear-usuario.component';
import { OlvideContraseniaComponent } from './componentes/olvide-contrasenia/olvide-contrasenia.component';
import { InicioComponent } from './componentes/inicio/inicio.component';
import { InicioAdminComponent } from './componentes/inicio-admin/inicio-admin.component';
import { DardeAltaComponent } from './componentes/darde-alta/darde-alta.component';
import { HabilitarComponent } from './componentes/habilitar/habilitar.component';
import { AdminCarpetasArchivosComponent } from './componentes/admin-carpetas-archivos/admin-carpetas-archivos.component';
import { NotFoundComponent } from './componentes/not-found/not-found.component';
import { InicioUsuarioComponent } from './componentes/inicio-usuario/inicio-usuario.component';
import { EditarCarpetasComponent } from './componentes/editar-carpetas/editar-carpetas.component';
import { ActualizarCuentaComponent } from './componentes/actualizar-cuenta/actualizar-cuenta.component';
import { EditarArchivosComponent } from './componentes/editar-archivos/editar-archivos.component';
import { SeePDFComponent } from './componentes/see-pdf/see-pdf.component';
import { DeniedComponent } from './componentes/denied/denied.component';

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
    children: [
      {
        path: 'EditarCarpetas',
        component: EditarCarpetasComponent,
      },
      {
        path: 'ActualizarCuenta',
        component: ActualizarCuentaComponent,
      },
      {
        path: 'EditarArchivo',
        component: EditarArchivosComponent,
      },
      {
        path: 'SeePDF',
        component: SeePDFComponent,
      },
    ]
  },
  {
    path: 'inicioAdmin',
    component: InicioAdminComponent,
  },
  {
    path: 'inicioUsuario',
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
  {
    path: 'denied',
    component: DeniedComponent,
  },
  {
    path: '**',
    component: NotFoundComponent,
  },
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
