import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { ReactiveFormsModule, FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { LoginComponent } from './componentes/login/login.component';
import { CrearUsuarioComponent } from './componentes/crear-usuario/crear-usuario.component';
import { OlvideContraseniaComponent } from './componentes/olvide-contrasenia/olvide-contrasenia.component';
import { InicioComponent } from './componentes/inicio/inicio.component';
import { InicioAdminComponent } from './componentes/inicio-admin/inicio-admin.component';
import { DardeAltaComponent } from './componentes/darde-alta/darde-alta.component';
import { HabilitarComponent } from './componentes/habilitar/habilitar.component';
import { AdminCarpetasArchivosComponent } from './componentes/admin-carpetas-archivos/admin-carpetas-archivos.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { MatSliderModule } from '@angular/material/slider';
import {MatFormFieldModule} from '@angular/material/form-field';
import {MatInputModule} from '@angular/material/input';
import {MatIconModule} from '@angular/material/icon';
import {MatStepperModule} from '@angular/material/stepper';
import {MatTabsModule} from '@angular/material/tabs';
import {MatToolbarModule} from '@angular/material/toolbar';
import {MatTableModule} from '@angular/material/table';
import {MatTooltipModule} from '@angular/material/tooltip';
import { NotFoundComponent } from './componentes/not-found/not-found.component';
import { InicioUsuarioComponent } from './componentes/inicio-usuario/inicio-usuario.component';
import {MatTreeModule} from '@angular/material/tree';
import {MatDialogModule} from '@angular/material/dialog';
import {MatRadioModule} from '@angular/material/radio';
import { EditarCarpetasComponent } from './componentes/editar-carpetas/editar-carpetas.component';
import { ActualizarCuentaComponent } from './componentes/actualizar-cuenta/actualizar-cuenta.component';
import { EditarArchivosComponent } from './componentes/editar-archivos/editar-archivos.component';
import { SeePDFComponent } from './componentes/see-pdf/see-pdf.component';
import { DeniedComponent } from './componentes/denied/denied.component';

@NgModule({
  declarations: [
    AppComponent,
    LoginComponent,
    CrearUsuarioComponent,
    OlvideContraseniaComponent,
    InicioComponent,
    InicioAdminComponent,
    DardeAltaComponent,
    HabilitarComponent,
    AdminCarpetasArchivosComponent,
    NotFoundComponent,
    InicioUsuarioComponent,
    EditarCarpetasComponent,
    ActualizarCuentaComponent,
    EditarArchivosComponent,
    SeePDFComponent,
    DeniedComponent
  ],
  entryComponents:[]
  ,
  imports: [
    BrowserModule,
    AppRoutingModule,
    ReactiveFormsModule,
    FormsModule,
    HttpClientModule,
    BrowserAnimationsModule,
    MatSliderModule,
    MatInputModule,
    MatFormFieldModule,
    MatIconModule,
    MatStepperModule,
    MatTabsModule,
    MatToolbarModule,
    MatTableModule,
    MatTooltipModule,
    MatTreeModule,
    MatDialogModule,
    MatRadioModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
